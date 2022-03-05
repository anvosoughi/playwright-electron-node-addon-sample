const { contextBridge } = require("electron");
const PLATFORM = require("os").platform();

const loadLib = () => {
  if (PLATFORM === "win32") {
    return require(`./libs/Test_Module_Windows.node`);
  }

  return require(`./libs/Test_Module_Mac.node`);
};

contextBridge.exposeInMainWorld("helloWorld", {
  testModule: loadLib(),
});
