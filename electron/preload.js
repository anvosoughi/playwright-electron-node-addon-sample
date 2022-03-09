const { contextBridge } = require("electron");
const PLATFORM = require("os").platform();

const loadLib = () => {
  if (PLATFORM === "win32") {
		console.log('loading windows lib');
    const test = require(`./libs/Test_Module_Windows.node`);
		console.log('found windows lib');
		return test;
  }

	// console.log('should not get here', test);
  // return require(`./libs/Test_Module_Mac.node`);
};

contextBridge.exposeInMainWorld("helloWorld", {
  testModule: loadLib(),
});
