helloWorld.testModule.add_callback((result) => {
  console.log("callback result: ", result);
});

const query = () => {
  const result = helloWorld.testModule.query();
  console.log("query result: ", result);
};

const triggerCallback = () => {
	console.log("triggerCallback fired");
  helloWorld.testModule.trigger_callback();
};

//trigger callback now!
triggerCallback();
