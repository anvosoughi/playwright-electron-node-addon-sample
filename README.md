# playwright-electron-node-addon-sample

**Steps:**

1. install root dependencies
`npm install`

2. Install playwright dependencies
- `cd e2e`

- `npm install`

3. run tests
`npm run test:e2e`

**Desccription:**

I have a simple Electron sample app, which uses the 'node-addon-api' to make a call from JavaScript(electron app) to C++ add-on, and receive a callback from C++ add-on to the JavaScript(electron app).

When the app is launched, the triggerCallback function is automatically executed, which triggers C++ add-on to make one call to the app every second using a callback.

There is a console output of "triggerCallback fired" when the function is executed. And "callback result: XX" is printed to console every second thereafter.
