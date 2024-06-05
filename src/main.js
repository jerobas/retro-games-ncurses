import wasm from './wasm/test.cjs';

const main = () => {
    wasm._myFunction()
}

wasm.onRuntimeInitialized = main