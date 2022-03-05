declare const helloWorld: {
  testModule: {
    add_callback: (callback: Function) => void;
    remove_callback: () => void;
    trigger_callback: () => void;
    query: () => number;
  };
};
