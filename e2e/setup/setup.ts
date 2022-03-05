import {_electron as electron, ElectronApplication} from 'playwright';

export async function launchApp(): Promise<ElectronApplication> {
  //run sample
  let electronApp: ElectronApplication;

   // Launch Electron app.
   electronApp = await electron.launch({ args: ['../electron/main.js'] });

  //  // Evaluation expression in the Electron context.
  //  const appPath = await electronApp.evaluate(async ({ app }) => {
  //    // This runs in the main Electron process, parameter here is always
  //    // the result of the require('electron') in the main app script.
  //    return app.getAppPath();
  //  });
  //  console.log(appPath);

   return electronApp;
}

export async function openDevTools(app: ElectronApplication): Promise<void> {
  const page = await app.firstWindow();
  const bwHandle = await app.browserWindow(page);
  await bwHandle.evaluate((bw) => bw.webContents.openDevTools());
}
