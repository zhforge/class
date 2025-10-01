/*
I/O operation:
READ = 10 Read a word from the keyboard into a specific location in memory.
WRITE = 11 Write a word from a specific location in memory to screen.
*/

const readline = require('node:readline');
const { stdin: input, stdout: output } = require('node.process');

const rl = readline.createInterface({ input, output });

/**
 * Read a word from the console into a specific location in memory.
 * @constructor
 * @param {number} address Memory location input will be stored at
 */
function read(address) {
    rl.question('', (input) => {
        // As in LC-3, no prompt is given for a keyboard input instruction
        const word = parseFloat(input);

        // Verify the input is a valid word
        if (Number.isInteger(word) && word >= -9999 && word <= 9999) {
            // Write word to memory
            // TODO: memory[address] = word
        }
    });
}

/**
 * Write a word from a specific location in memory to console.
 * @constructor
 * @param {number} address Memory location writing from
 */
function write(address) {
    // TODO: console.log(memory[address])
}