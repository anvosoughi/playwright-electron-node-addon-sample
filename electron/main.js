const { app, BrowserWindow } = require("electron");
const path = require("path");
const env = process.env.NODE_ENV || "development";

const createWindow = () => {
  let mainWindow = new BrowserWindow({
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
    },
  });

  mainWindow.loadFile(path.join(__dirname, "../web/public/index.html"));
  mainWindow.once("ready-to-show", () => mainWindow.show());
  mainWindow.on("closed", () => (mainWindow = null));
  mainWindow.webContents.openDevTools();

  return mainWindow;
};

app.whenReady().then(() => {
  createWindow();
});
