const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const app = express();

app.use(express.json());
app.use(express.static('.'));

app.post('/check', (req, res) => {
    const word = req.body.text;
    
    exec(`echo ${word} | .\\checker.exe`, (error, stdout) => {
        if (error) {
            console.error("C Program Error:", error);
            return res.json({ correct: false });
        }
        
        const output = stdout.toLowerCase();
        const isCorrect = output.includes("correct") && !output.includes("incorrect");
        
        res.json({ correct: isCorrect });
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`\n SERVER STARTED!`);
    console.log(` Open your browser to: http://localhost:${PORT}`);
    console.log(`Press Ctrl+C to stop the server.\n`);
});