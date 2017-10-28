const fs = require("fs");
const path = require("path");
const JSZip = require("jszip");

const PACKAGE_JSON = require(path.join(__dirname, "..", "package.json"))
const FIRM_VERSION = PACKAGE_JSON.version;
const OUTPUT_FILES = path.join(__dirname, "..", "output_files");
const RELEASE_JSON = "release.json";
const info = JSON.parse(fs.readFileSync(path.join(__dirname, RELEASE_JSON)));

const OUTPUT_ZIP = `${PACKAGE_JSON.name}-${FIRM_VERSION}.zip`;
const RPD_IMG1 = path.join(OUTPUT_FILES, "olive_std_top_cfm1_auto.rpd");
const RPD_UFM = path.join(OUTPUT_FILES, "olive_std_top_ufm_auto.rpd");

console.info(`Packaging firmware (Version ${FIRM_VERSION})`);
let zip = new JSZip();
info.variations.reduce((promise, variation) => {
    let name = variation.path;
    return promise
    .then(() => {
        let base = path.basename(name, ".zip");
        let src = path.join(__dirname, base, base + "-compressed.elf");
        let data;
        if (fs.existsSync(src)) {
            data = fs.readFileSync(src);
        } else {
            src = path.join(__dirname, base, base + ".elf");
            data = fs.readFileSync(src);
        }
        let zip_in = new JSZip();
        console.info("- Adding " + name + " (" + data.length + " bytes)");
        zip_in.file("spi.elf", data);
        zip_in.file("image1.rpd", fs.readFileSync(RPD_IMG1));
        zip_in.file("ufm.rpd", fs.readFileSync(RPD_UFM));
        return zip_in.generateAsync({type: "nodebuffer", compression: "DEFLATE"});
    })
    .then((content) => {
        zip.file(name, content);
    });
}, Promise.resolve())
.then(() => {
    zip.file(RELEASE_JSON, JSON.stringify(info));
    console.info("- Compressing...");
    return zip.generateAsync({
        compression: "DEFLATE",
        type: "nodebuffer"
    });
})
.then((content) => {
    console.info("- Writing " + OUTPUT_ZIP + " (" + content.length + " bytes)");
    fs.writeFileSync(OUTPUT_ZIP, content);
});
