const fs = require("fs")
const crypto = require('crypto')
const path = require("path")
var stdinBuffer = fs.readFileSync(0)
const input = stdinBuffer.toString().split('\n').filter(_ => !(_.startsWith('cc1:') || _.startsWith('make[') || _.startsWith('make:')))
const root = path.resolve(process.argv.pop())

//input.forEach(_ => console.log('::: ' + _))
const res = []
input.forEach(line => {
    const regex = /^(\/.*?):([0-9]+):([0-9]+):\s*([a-z]+):\s*(.*)/m;
    let m;
    if ((m = regex.exec(line)) !== null) {
        const [all, path, lin, col, level, message] = m
        if (level == 'note' || path.indexOf('third-party') > 0) return
        const id = crypto.createHash('sha256')
            .update(path.substring(root.length + 1), 'utf8')
            .update(message, 'utf8')
            .digest('hex')
        res.push({
            id,
            category: 'sast',
            message,
            cve: '',
            scanner: {
                id: 'gcc11',
                name: 'gcc11'
            },
            description: '',
            severity: level == 'note' ? 'Info' : (level == 'warning' ? 'Info' : 'Critical'), // info, minor, major, critical, or blocker
            location: {
                file: path.substring(root.length + 1),
                start_line: parseInt(lin)
            },
            confidence: 'High',
            identifiers: [{
                "type": "gcc11",
                "name": "GCC warning",
                "value": "gcc"
            }]
        })
    }
    else if (res.length) res[res.length - 1].description += '\n' + line
})
console.log(JSON.stringify({ version: '14.0.4', vulnerabilities: res }, null, 2))
if (res.length) process.exitCode = 1
