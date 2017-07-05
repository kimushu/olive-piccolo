const fs = require("fs");
const path = require("path");
const JSZip = require("jszip");

const RELEASE_JSON = "release.json";
const info = JSON.parse(fs.readFileSync(path.join(__dirname, RELEASE_JSON)));

const OUTPUT_ZIP = "data.zip";

let zip = new JSZip();
info.variations.forEach((variation) => {
    let name = variation.path;
    let base = path.basename(name, ".elf");
    let src = path.join(__dirname, base, base + "-compressed.elf");
    let data;
    if (fs.existsSync(src)) {
        data = fs.readFileSync(src);
    } else {
        src = path.join(__dirname, base, name);
        data = fs.readFileSync(src);
    }
    console.info("Adding " + name + " (" + data.length + " bytes)");
    zip.file(name, data);
});
zip.file(RELEASE_JSON, JSON.stringify(info));
console.info("Compressing...");
zip.generateAsync({
    compression: "DEFLATE",
    type: "nodebuffer"
})
.then((content) => {
    console.info("Writing " + OUTPUT_ZIP + " (" + content.length + " bytes)");
    fs.writeFileSync(OUTPUT_ZIP, content);
});
