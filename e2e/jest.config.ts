module.exports = {
  moduleFileExtensions: ['js', 'ts', 'jsx', 'json'],
  transform: {'\\.[jt]sx?$': 'ts-jest'},
  setupFilesAfterEnv: ['./setup/jest.setup.ts'],
  testTimeout: 120000,
  testRegex: '(/specs/.*spec)\\.[jt]s?$',
  verbose: true,
  reporters: ['default'],
  testRunner: 'jasmine2', // jasmine runner is explicitly added to support jest-allure package otherwise jest-circus is the default test runner of jest 27
};
