
const fs = require('fs')
const path = require('path')

const test_folder = path.join(__dirname, 'testset')
const output_folder = path.join(__dirname, 'output')

async function ReadInput(fname) {
	return fs.readFileSync(fname, 'utf-8').split('\r\n').splice(2).map(d => {
		let j = d.split(' ')
		return { name: j[0], start: parseInt(j[1]), duration: parseInt(j[2]) }
	})
}

async function GetNamePidMap(fname) {
	let i = fs.readFileSync(fname, 'utf-8').split('\n').slice(0, -1)
	let j = {}
	for (const k of i) {
		l = k.split(' ')
		j[l[0]] = parseInt(l[1])
	}
	return j
}

async function GetResult(fname) {
	let r = fs.readFileSync(fname, 'utf-8').split('\n').slice(0, -1).map(i => i.split(' ').splice(-3, 3).map(x => parseFloat(x)))
	let mintime = 1e20
	for (const k of r) mintime = Math.min(mintime, k[1], k[2])
	let u = {}
	for (let k of r) u[k[0]] = { start: k[1] - mintime, end: k[2] - mintime }
	return u
}

async function CalcTimeUnit() {
	let inp = await ReadInput(path.join(test_folder, 'TIME_MEASUREMENT.txt'))
	let mp = await GetNamePidMap(path.join(output_folder, 'TIME_MEASUREMENT_stdout.txt'))
	let exet = await GetResult(path.join(output_folder, 'TIME_MEASUREMENT_dmesg.txt'))
	return exet[mp['P9']].end / (inp[9].start + inp[9].duration)
}


;(async () => {

const TIME_UNIT = await CalcTimeUnit()
console.log(`Realtime unit = ${TIME_UNIT} seconds`)

const tests = await fs.readdirSync(test_folder)

for (let input of tests) {
	let test = input.replace(/\.[^/.]+$/, "")

	if (test === 'TIME_MEASUREMENT') continue

	let output = `${test}_stdout.txt`
	let kern = `${test}_dmesg.txt`

	console.log(`Judging test ${test}`)

	let inp = await ReadInput(path.join(test_folder, input))
	let mp = await GetNamePidMap(path.join(output_folder, output))
	let exet = await GetResult(path.join(output_folder, kern))

	console.log('inp', inp)
	console.log('mp', mp)
	console.log('exet', exet)
}

}) ()
