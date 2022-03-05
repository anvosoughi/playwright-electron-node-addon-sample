helloWorld.testModule.add_callback((result) => {
  console.log("callback result: ", result);
});

const query = () => {
  const result = helloWorld.testModule.query();
  console.log("query result: ", result);
};

const triggerCallback = () => {
	console.log("triggerCallback clicked: ");
  helloWorld.testModule.trigger_callback();
};
