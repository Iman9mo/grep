var __create = Object.create;
var __defProp = Object.defineProperty;
var __getOwnPropDesc = Object.getOwnPropertyDescriptor;
var __getOwnPropNames = Object.getOwnPropertyNames;
var __getProtoOf = Object.getPrototypeOf;
var __hasOwnProp = Object.prototype.hasOwnProperty;
var __copyProps = (to, from, except, desc) => {
  if (from && typeof from === "object" || typeof from === "function") {
    for (let key of __getOwnPropNames(from))
      if (!__hasOwnProp.call(to, key) && key !== except)
        __defProp(to, key, { get: () => from[key], enumerable: !(desc = __getOwnPropDesc(from, key)) || desc.enumerable });
  }
  return to;
};
var __toESM = (mod, isNodeMode, target) => (target = mod != null ? __create(__getProtoOf(mod)) : {}, __copyProps(
  isNodeMode || !mod || !mod.__esModule ? __defProp(target, "default", { value: mod, enumerable: true }) : target,
  mod
));

// src-electron/electron-main.js
var import_electron = require("electron");
var import_path = __toESM(require("path"));
var import_os = __toESM(require("os"));
var platform = process.platform || import_os.default.platform();
var mainWindow;
function createWindow() {
  mainWindow = new import_electron.BrowserWindow({
    icon: import_path.default.resolve(__dirname, "icons/icon.png"),
    width: 1e3,
    height: 600,
    useContentSize: true,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: true,
      preload: import_path.default.resolve(__dirname, "C:\\Users\\Lenovo\\Desktop\\os\\main\\UI\\.quasar\\electron\\electron-preload.js")
    }
  });
  mainWindow.loadURL("http://localhost:9300");
  if (true) {
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.webContents.on("devtools-opened", () => {
      mainWindow.webContents.closeDevTools();
    });
  }
  mainWindow.on("closed", () => {
    mainWindow = null;
  });
}
import_electron.app.whenReady().then(createWindow);
import_electron.app.on("window-all-closed", () => {
  if (platform !== "darwin") {
    import_electron.app.quit();
  }
});
import_electron.app.on("activate", () => {
  if (mainWindow === null) {
    createWindow();
  }
});
var { exec } = require("child_process");
import_electron.ipcMain.on("execMain", (event, { folderLocation, regex, searchedChar }) => {
  const fileName = "./main";
  exec(
    `${fileName} ${folderLocation} ${regex} ${searchedChar}`,
    (error, stdout, stderr) => {
      event.sender.send("result", stdout);
    }
  );
});
//# sourceMappingURL=data:application/json;base64,ewogICJ2ZXJzaW9uIjogMywKICAic291cmNlcyI6IFsiLi4vLi4vc3JjLWVsZWN0cm9uL2VsZWN0cm9uLW1haW4uanMiXSwKICAic291cmNlc0NvbnRlbnQiOiBbImltcG9ydCB7IGFwcCwgQnJvd3NlcldpbmRvdywgaXBjTWFpbiB9IGZyb20gXCJlbGVjdHJvblwiO1xyXG5pbXBvcnQgcGF0aCBmcm9tIFwicGF0aFwiO1xyXG5pbXBvcnQgb3MgZnJvbSBcIm9zXCI7XHJcblxyXG4vLyBuZWVkZWQgaW4gY2FzZSBwcm9jZXNzIGlzIHVuZGVmaW5lZCB1bmRlciBMaW51eFxyXG5jb25zdCBwbGF0Zm9ybSA9IHByb2Nlc3MucGxhdGZvcm0gfHwgb3MucGxhdGZvcm0oKTtcclxuXHJcbmxldCBtYWluV2luZG93O1xyXG5cclxuZnVuY3Rpb24gY3JlYXRlV2luZG93KCkge1xyXG4gIC8qKlxyXG4gICAqIEluaXRpYWwgd2luZG93IG9wdGlvbnNcclxuICAgKi9cclxuICBtYWluV2luZG93ID0gbmV3IEJyb3dzZXJXaW5kb3coe1xyXG4gICAgaWNvbjogcGF0aC5yZXNvbHZlKF9fZGlybmFtZSwgXCJpY29ucy9pY29uLnBuZ1wiKSwgLy8gdHJheSBpY29uXHJcbiAgICB3aWR0aDogMTAwMCxcclxuICAgIGhlaWdodDogNjAwLFxyXG4gICAgdXNlQ29udGVudFNpemU6IHRydWUsXHJcbiAgICB3ZWJQcmVmZXJlbmNlczoge1xyXG4gICAgICBub2RlSW50ZWdyYXRpb246IHRydWUsXHJcbiAgICAgIGNvbnRleHRJc29sYXRpb246IHRydWUsXHJcbiAgICAgIC8vIE1vcmUgaW5mbzogaHR0cHM6Ly92Mi5xdWFzYXIuZGV2L3F1YXNhci1jbGktdml0ZS9kZXZlbG9waW5nLWVsZWN0cm9uLWFwcHMvZWxlY3Ryb24tcHJlbG9hZC1zY3JpcHRcclxuICAgICAgcHJlbG9hZDogcGF0aC5yZXNvbHZlKF9fZGlybmFtZSwgcHJvY2Vzcy5lbnYuUVVBU0FSX0VMRUNUUk9OX1BSRUxPQUQpLFxyXG4gICAgfSxcclxuICB9KTtcclxuXHJcbiAgbWFpbldpbmRvdy5sb2FkVVJMKHByb2Nlc3MuZW52LkFQUF9VUkwpO1xyXG5cclxuICBpZiAocHJvY2Vzcy5lbnYuREVCVUdHSU5HKSB7XHJcbiAgICAvLyBpZiBvbiBERVYgb3IgUHJvZHVjdGlvbiB3aXRoIGRlYnVnIGVuYWJsZWRcclxuICAgIG1haW5XaW5kb3cud2ViQ29udGVudHMub3BlbkRldlRvb2xzKCk7XHJcbiAgfSBlbHNlIHtcclxuICAgIC8vIHdlJ3JlIG9uIHByb2R1Y3Rpb247IG5vIGFjY2VzcyB0byBkZXZ0b29scyBwbHNcclxuICAgIG1haW5XaW5kb3cud2ViQ29udGVudHMub24oXCJkZXZ0b29scy1vcGVuZWRcIiwgKCkgPT4ge1xyXG4gICAgICBtYWluV2luZG93LndlYkNvbnRlbnRzLmNsb3NlRGV2VG9vbHMoKTtcclxuICAgIH0pO1xyXG4gIH1cclxuXHJcbiAgbWFpbldpbmRvdy5vbihcImNsb3NlZFwiLCAoKSA9PiB7XHJcbiAgICBtYWluV2luZG93ID0gbnVsbDtcclxuICB9KTtcclxufVxyXG5cclxuYXBwLndoZW5SZWFkeSgpLnRoZW4oY3JlYXRlV2luZG93KTtcclxuXHJcbmFwcC5vbihcIndpbmRvdy1hbGwtY2xvc2VkXCIsICgpID0+IHtcclxuICBpZiAocGxhdGZvcm0gIT09IFwiZGFyd2luXCIpIHtcclxuICAgIGFwcC5xdWl0KCk7XHJcbiAgfVxyXG59KTtcclxuXHJcbmFwcC5vbihcImFjdGl2YXRlXCIsICgpID0+IHtcclxuICBpZiAobWFpbldpbmRvdyA9PT0gbnVsbCkge1xyXG4gICAgY3JlYXRlV2luZG93KCk7XHJcbiAgfVxyXG59KTtcclxuXHJcbmNvbnN0IHsgZXhlYyB9ID0gcmVxdWlyZShcImNoaWxkX3Byb2Nlc3NcIik7XHJcblxyXG5pcGNNYWluLm9uKFwiZXhlY01haW5cIiwgKGV2ZW50LCB7IGZvbGRlckxvY2F0aW9uLCByZWdleCwgc2VhcmNoZWRDaGFyIH0pID0+IHtcclxuICBjb25zdCBmaWxlTmFtZSA9IFwiLi9tYWluXCI7XHJcblxyXG4gIGV4ZWMoXHJcbiAgICBgJHtmaWxlTmFtZX0gJHtmb2xkZXJMb2NhdGlvbn0gJHtyZWdleH0gJHtzZWFyY2hlZENoYXJ9YCxcclxuICAgIChlcnJvciwgc3Rkb3V0LCBzdGRlcnIpID0+IHtcclxuICAgICAgZXZlbnQuc2VuZGVyLnNlbmQoXCJyZXN1bHRcIiwgc3Rkb3V0KTtcclxuICAgIH1cclxuICApO1xyXG59KTtcclxuIl0sCiAgIm1hcHBpbmdzIjogIjs7Ozs7Ozs7Ozs7Ozs7Ozs7Ozs7QUFBQSxzQkFBNEM7QUFDNUMsa0JBQWlCO0FBQ2pCLGdCQUFlO0FBR2YsSUFBTSxXQUFXLFFBQVEsWUFBWSxVQUFBQSxRQUFHLFNBQVM7QUFFakQsSUFBSTtBQUVKLFNBQVMsZUFBZTtBQUl0QixlQUFhLElBQUksOEJBQWM7QUFBQSxJQUM3QixNQUFNLFlBQUFDLFFBQUssUUFBUSxXQUFXLGdCQUFnQjtBQUFBLElBQzlDLE9BQU87QUFBQSxJQUNQLFFBQVE7QUFBQSxJQUNSLGdCQUFnQjtBQUFBLElBQ2hCLGdCQUFnQjtBQUFBLE1BQ2QsaUJBQWlCO0FBQUEsTUFDakIsa0JBQWtCO0FBQUEsTUFFbEIsU0FBUyxZQUFBQSxRQUFLLFFBQVEsV0FBVyxrRkFBbUM7QUFBQSxJQUN0RTtBQUFBLEVBQ0YsQ0FBQztBQUVELGFBQVcsUUFBUSx1QkFBbUI7QUFFdEMsTUFBSSxNQUF1QjtBQUV6QixlQUFXLFlBQVksYUFBYTtBQUFBLEVBQ3RDLE9BQU87QUFFTCxlQUFXLFlBQVksR0FBRyxtQkFBbUIsTUFBTTtBQUNqRCxpQkFBVyxZQUFZLGNBQWM7QUFBQSxJQUN2QyxDQUFDO0FBQUEsRUFDSDtBQUVBLGFBQVcsR0FBRyxVQUFVLE1BQU07QUFDNUIsaUJBQWE7QUFBQSxFQUNmLENBQUM7QUFDSDtBQUVBLG9CQUFJLFVBQVUsRUFBRSxLQUFLLFlBQVk7QUFFakMsb0JBQUksR0FBRyxxQkFBcUIsTUFBTTtBQUNoQyxNQUFJLGFBQWEsVUFBVTtBQUN6Qix3QkFBSSxLQUFLO0FBQUEsRUFDWDtBQUNGLENBQUM7QUFFRCxvQkFBSSxHQUFHLFlBQVksTUFBTTtBQUN2QixNQUFJLGVBQWUsTUFBTTtBQUN2QixpQkFBYTtBQUFBLEVBQ2Y7QUFDRixDQUFDO0FBRUQsSUFBTSxFQUFFLEtBQUssSUFBSSxRQUFRO0FBRXpCLHdCQUFRLEdBQUcsWUFBWSxDQUFDLE9BQU8sRUFBRSxnQkFBZ0IsT0FBTyxhQUFhLE1BQU07QUFDekUsUUFBTSxXQUFXO0FBRWpCO0FBQUEsSUFDRSxHQUFHLFlBQVksa0JBQWtCLFNBQVM7QUFBQSxJQUMxQyxDQUFDLE9BQU8sUUFBUSxXQUFXO0FBQ3pCLFlBQU0sT0FBTyxLQUFLLFVBQVUsTUFBTTtBQUFBLElBQ3BDO0FBQUEsRUFDRjtBQUNGLENBQUM7IiwKICAibmFtZXMiOiBbIm9zIiwgInBhdGgiXQp9Cg==
