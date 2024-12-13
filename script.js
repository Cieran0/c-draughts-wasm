const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
let wasmModule;

const wasmImports = {
    env: {
        js_draw_rectangle: (x, y, width, height, colour) => {
            console.log(x, y, width, height, colour);
            ctx.fillStyle = `#${colour.toString(16).padStart(6, '0')}`;
            ctx.fillRect(x, y, width, height);
        },
        js_draw_circle: (x, y, radius, colour) => {
            console.log(x, y, radius, colour);

            ctx.beginPath();
            ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
            ctx.fillStyle = `#${colour.toString(16).padStart(6, '0')}`;
            ctx.fill();
        },
        js_log: (stringPtr) => {
            const memory = wasmModule.instance.exports.memory;
            let length = 0;
            const maxLength = 1024; // Max string length (to avoid infinite loops in case of errors)
            while (length < maxLength) {
                const byte = new Uint8Array(memory.buffer, stringPtr + length, 1)[0];
                if (byte === 0) break;  // Null byte indicates end of string
                length++;
            }
            const stringArray = new Uint8Array(memory.buffer, stringPtr, length);
            const string = new TextDecoder().decode(stringArray);
            console.log(string);
        }
    }
};

// Load and initialize the WebAssembly module
async function initializeWASM() {
    // Wait until the WASM module is loaded and instantiated
    const response = await fetch('main.wasm');
    const buffer = await response.arrayBuffer();
    wasmModule = await WebAssembly.instantiate(buffer, wasmImports);

}

// Wait for WASM to be initialized before doing anything else
async function startApp() {
    await initializeWASM();  // Ensure WASM is initialized before continuing

    // After WASM is ready, you can do further tasks
    console.log('WASM is initialized and ready!');
    wasmModule.instance.exports.reset();


    window.addEventListener('resize', () => {
        resizeCanvas();
    });
    
    // Initialize canvas and draw something
    resizeCanvas();
}

// Function to resize the canvas to match the screen resolution
function resizeCanvas() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    console.log("X:" + canvas.width + " Y: " + canvas.height);

    canvas.style.width = `${window.innerWidth}px`;
    canvas.style.height = `${window.innerHeight}px`;

    wasmModule.instance.exports.on_resize(canvas.width, canvas.height);
}




// Start the app once everything is ready
startApp();
