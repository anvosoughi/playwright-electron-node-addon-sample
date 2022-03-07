import {launchApp} from '../setup/setup';
import {ElectronApplication} from 'playwright';

let app: ElectronApplication;

describe('Section 1', function () {
  beforeAll(async () => {
    app = await launchApp();
  });

  afterAll(async () => {
    await app.close();
  });

  it('t1', async () => {
     const page = await app.firstWindow();
     //wait 3 seconds so app is loaded... otherwise sometimes the button click fails...
     //await page.waitForTimeout(3000);

     //click on Trigger Callback
    //  const callbackBtn = await page.waitForSelector('button:has-text("Trigger Callback")');
    //  await callbackBtn.click();

     //keep app open for another 10 seconds
     await page.waitForTimeout(10000);
  });
});
