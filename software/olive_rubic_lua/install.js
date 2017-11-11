const path = require('path');
const fs = require('fs-extra');
const download = require('download');
const decompress = require('decompress');
const decompressTarxz = require('decompress-tarxz');
const decompressTargz = require('decompress-targz');
const JsDiff = require('diff');

const DEST_DIR = path.join(__dirname, 'lib');
const LUA_VER = '5.3.4';

function downloadIfNotExist(url, dest) {
    let name = url.split('/').slice(-1)[0];
    let file = path.join(dest, name);
    return fs.exists(file)
    .then((exists) => {
        if (!exists) {
            console.log(`Downloading ${url}`);
            return download(url, dest);
        }
    })
    .then(() => {
        return file;
    })
}

function extract(archive, dir, includes, excludes, dest) {
    return decompress(archive, {
        plugins: [
            decompressTarxz(), decompressTargz()
        ]
    })
    .then((a_files) => {
        return a_files.reduce((promise, file) => {
            let temp = file.path.split('/').slice(1);
            if (file.type !== "file") {
                return promise;
            }
            let a_file = temp.pop();
            if ((dir != null) && (temp.shift() !== dir)) {
                return promise;
            }
            let a_dir = temp.join("/");
            let a_path = (a_dir !== "") ? `${a_dir}/${a_file}` : a_file;
            if (excludes instanceof Array) {
                if (excludes.indexOf(a_path) >= 0) {
                    return promise;
                }
            } else if (excludes instanceof RegExp) {
                if (excludes.exec(a_path) != null) {
                    return promise;
                }
            }
            if (includes instanceof Array) {
                if (includes.indexOf(a_path) < 0) {
                    return promise;
                }
            } else if (includes instanceof RegExp) {
                if (includes.exec(a_path) == null) {
                    return promise;
                }
            }
            return promise
            .then(() => {
                console.log(`Extracting ${a_path} ...`);
                let target = path.join(dest, a_path);
                return fs.ensureDir(path.dirname(target))
                .then(() => {
                    return fs.writeFile(target, file.data);
                });
            });
        }, Promise.resolve());
    });
}

fs.ensureDir(DEST_DIR)
.then(() => {
    return downloadIfNotExist(
        `https://www.lua.org/ftp/lua-${LUA_VER}.tar.gz`,
        DEST_DIR
    )
    .then((archive) => {
        return extract(archive, 'src', /\.[ch]$/, /^luac?\.c$/, path.join(DEST_DIR, 'lua'));
    });
})
.then(() => {
    return [
        ['lua', 'luaconf.h'],
    ].reduce((promise, target) => {
        let file = path.join(DEST_DIR, ...target);
        return promise
        .then(() => {
            return Promise.all([
                fs.readFile(file, "utf8"),
                fs.readFile(path.join(__dirname, target[2] || `${target[1]}.patch`), "utf8"),
            ]);
        })
        .then(([input, patch]) => {
            console.log(`Applying patch to ${target[1]} ...`);
            result = JsDiff.applyPatch(input, patch);
            if (result === false) {
                throw new Error("Patch failed!");
            }
            return fs.writeFile(file, result, "utf8");
        });
    }, Promise.resolve());
})
.then(() => {
    console.log('done!');
}, (reason) => {
    console.error(reason);
    process.exitCode = 1;
});
