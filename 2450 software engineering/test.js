process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.on('data', data => {
    // As in LC-3, no prompt is given for a keyboard input instruction
    console.log(data)
    const word = parseFloat(data.trim());
    console.log(word)

    // Verify the input is a valid word
    if (Number.isInteger(word) && word >= -9999 && word <= 9999) {
        // Write word to memory
        //vm.memory.setLoc(v, word);
        console.log(word)
    }
});