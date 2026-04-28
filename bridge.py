from flask import Flask, request, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__)

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/check', methods=['POST'])
def check():
    word = request.json.get('text', '')
    process = subprocess.Popen(['./checker.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, _ = process.communicate(input=f"{word}\n")
    
    is_correct = "Correct" in stdout
    return jsonify({"correct": is_correct})

if __name__ == '__main__':
    print("Server running! Go to http://127.0.0.1:5000")
    app.run(port=5000)