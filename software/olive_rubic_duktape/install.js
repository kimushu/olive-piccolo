const path = require('path');
const fs = require('fs-extra');
const download = require('download');
const decompress = require('decompress');
const decompressTarxz = require('decompress-tarxz');
const decompressTargz = require('decompress-targz');
const JsDiff = require('diff');

const DEST_DIR = path.join(__dirname, 'lib');
const DUK_VER = '2.2.0';
const DUX_VER = '0.1.1';

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

function extract(archive, dir, files, dest) {
    return decompress(archive, {
        plugins: [
            decompressTarxz(), decompressTargz()
        ]
    })
    .then((a_files) => {
        return a_files.reduce((promise, file) => {
            let temp = file.path.split('/').slice(1);
            let a_file = temp.pop();
            let a_dir = temp.join('/');
            if ((a_dir !== dir) || (files.indexOf(a_file) < 0)) {
                return promise;
            }
            return promise
            .then(() => {
                console.log(`Extracting ${a_file} ...`);
                return fs.writeFile(path.join(dest, a_file), file.data);
            });
        }, fs.ensureDir(dest));
    });
}

fs.ensureDir(DEST_DIR)
.then(() => {
    return downloadIfNotExist(
        `https://github.com/svaarala/duktape/releases/download/v${DUK_VER}/duktape-${DUK_VER}.tar.xz`,
        DEST_DIR
    )
    .then((archive) => {
        return extract(archive, 'src', [
            'duk_config.h', 'duktape.c', 'duktape.h'
        ], path.join(DEST_DIR, 'duktape'));
    });
})
.then(() => {
    return downloadIfNotExist(
        `https://github.com/kimushu/duktape-extension/releases/download/v${DUX_VER}/duktape-extension-${DUX_VER}.tar.gz`,
        DEST_DIR
    )
    .then((archive) => {
        return extract(archive, 'dist', [
            'dukext.c', 'dukext.h', 'dux_config.h'
        ], path.join(DEST_DIR, 'duktape-extension'));
    });
})
.then(() => {
    return [
        ['duktape', 'duk_config.h'],
        ['duktape-extension', 'dux_config.h'],
    ].reduce((promise, target) => {
        let file = path.join(DEST_DIR, ...target);
        return promise
        .then(() => {
            return Promise.all([
                fs.readFile(file, "utf8"),
                fs.readFile(path.join(__dirname, `${target[1]}.patch`), "utf8"),
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
