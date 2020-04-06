const PriorityQueue = () => {const top=0,parent=c=>(c+1>>>1)-1,left=c=>(c<<1)+1,right=c=>c+1<<1;class PriorityQueue{constructor(c=(d,e)=>d>e){this._heap=[],this._comparator=c}size(){return this._heap.length}isEmpty(){return 0==this.size()}peek(){return this._heap[top]}push(...c){return c.forEach(d=>{this._heap.push(d),this._siftUp()}),this.size()}pop(){const c=this.peek(),d=this.size()-1;return d>top&&this._swap(top,d),this._heap.pop(),this._siftDown(),c}replace(c){const d=this.peek();return this._heap[top]=c,this._siftDown(),d}_greater(c,d){return this._comparator(this._heap[c],this._heap[d])}_swap(c,d){[this._heap[c],this._heap[d]]=[this._heap[d],this._heap[c]]}_siftUp(){for(let c=this.size()-1;c>top&&this._greater(c,parent(c));)this._swap(c,parent(c)),c=parent(c)}_siftDown(){for(let d,c=top;left(c)<this.size()&&this._greater(left(c),c)||right(c)<this.size()&&this._greater(right(c),c);)d=right(c)<this.size()&&this._greater(right(c),left(c))?right(c):left(c),this._swap(c,d),c=d}}return PriorityQueue}

const fs = require('fs')
const path = require('path')

const test_folder = path.join(__dirname, 'testset')
const output_folder = path.join(__dirname, 'output')

async function ReadInput(fname) {
	let i = fs.readFileSync(fname, 'utf-8').replace(/\r\n$/gm, '').split('\r\n')
	return {
		type: i[0],
		jobs: i.splice(2).map(d => {
			let j = d.split(' ')
			return { name: j[0], start: parseInt(j[1]), duration: parseInt(j[2]) }
		})
	}
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

async function GetResult(fname, inp, TIME_UNIT) {
	let r = fs.readFileSync(fname, 'utf-8').split('\n').slice(0, -1).map(i => i.split(' ').splice(-3, 3).map(x => parseFloat(x)))
	let mintime = 1e20
	for (const k of r) mintime = Math.min(mintime, k[1], k[2])
	if (inp !== undefined) {
		let mintick = 1e20
		for (const k of inp.jobs) mintick = Math.min(mintick, k.start)
		mintime -= mintick * TIME_UNIT
	}
	let u = {}
	for (let k of r) u[k[0]] = { start: k[1] - mintime, end: k[2] - mintime }
	return u
}

async function CalcTimeUnit() {
	let inp = await ReadInput(path.join(test_folder, 'TIME_MEASUREMENT.txt'))
	let mp = await GetNamePidMap(path.join(output_folder, 'TIME_MEASUREMENT_stdout.txt'))
	let exet = await GetResult(path.join(output_folder, 'TIME_MEASUREMENT_dmesg.txt'))
	return exet[mp['P9']].end / (inp.jobs[9].start + inp.jobs[9].duration)
}

async function Simulation(td) {
	let njob = td.jobs.reduce((_, v, i, a) => { 
		_.push({ i, v })
		return _
	}, []).sort((a, b) => (a.v.start == b.v.start ? a.i - b.i : a.v.start - b.v.start))

	let rt = {}

	;({ FIFO: td => { 
		for (let i = 0, nt = 0; i < td.length; i++) {
			if (td[i].start > nt) nt = td[i].start
			rt[td[i].name] = { start: nt, end: nt + td[i].duration }
			nt += td[i].duration
		}
	}, RR: td => {
		let q = []
		let nt = 0, rrc = 500, it = 0
		while (1) {
			if (it == td.length && q.length == 0) break

			if (q.length && q[0].duration == 0) {
				rrc = 500
				rt[q[0].name] = { start: q[0].start, end: nt }
				q.splice(0, 1)
			}

			while (it < td.length && td[it].start <= nt) {
				q.push({ name: td[it].name, start: -1, duration: td[it].duration })
				it++
			}

			if (rrc == 0) {
				let z = q.splice(0, 1)
				q.push(z[0])
				rrc = 500
			}

			if (q.length) {
				q[0].duration--
				rrc--
				if (q[0].start == -1) q[0].start = nt
			}

			nt++
		}
	}, SJF: td => {
		let pq = new (PriorityQueue())((a, b) => { return a._.duration == b._.duration ? a.i < b.i : a._.duration < b._.duration })
		let job = undefined
		let nt = 0, it = 0
		while (1) {
			if (pq.isEmpty() && job === undefined && it == td.length) break

			if (job && job.duration == 0) {
				rt[job.name] = { start: job.start, end: nt }
				job = undefined
			}

			while (it < td.length && td[it].start <= nt) {
				pq.push({ i: it, _: td[it] })
				it++
			}

			if (job === undefined && pq.size()) {
				job = pq.pop()._
				job.start = nt
			}

			++nt
			if (job !== undefined) {
				job.duration--
			}
		}
	}, PSJF: td => {
		let pq = new (PriorityQueue())((a, b) => { return a.duration < b.duration })
		let job = undefined
		let nt = 0, it = 0
		while (1) {
			if (pq.isEmpty() && job === undefined && it == td.length) break

			if (job && job.duration == 0) {
				rt[job.name] = { start: job.start, end: nt }
				job = undefined
			}

			while (it < td.length && td[it].start <= nt) {
				pq.push({ name: td[it].name, start: -1, duration: td[it].duration })
				if (job !== undefined && job.duration > td[it].duration) {
					pq.push(job)
					job = undefined
				}
				it++
			}

			if (job === undefined && pq.size()) {
				job = pq.pop()
				if (job.start == -1) job.start = nt
			}

			++nt
			if (job !== undefined) {
				job.duration--
			}
		}
	} }[td.type](JSON.parse(JSON.stringify(td.jobs))))

	return rt
}

async function Judge(test, res, thr, thr_t, imp, TIME_UNIT) {
	Object.keys(imp).forEach(i => {
		let pname = imp[i]
		let exp_start = thr[pname].start
		let exp_end = thr[pname].end
		let start_t = res[i].start
		let end_t = res[i].end
		let start_tick = start_t / TIME_UNIT
		let end_tick = end_t / TIME_UNIT
		let diff_s = (start_tick - exp_start) / exp_start
		let diff_e = (end_tick - exp_end) / exp_end
		if (isNaN(diff_s)) diff_s = 0
		console.log(`Process ${pname} expected start at tick ${exp_start}, started at tick ${start_tick.toFixed(2)} (${start_t.toFixed(2)} second) (difference ${(diff_s * 100).toFixed(2)}%)`)
		console.log(`Process ${pname} expected  end  at tick ${exp_end}, started at tick ${end_tick.toFixed(2)} (${end_t.toFixed(2)} second) (difference ${(diff_e * 100).toFixed(2)}%)`)
	})
}

async function main() {
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
		let exet = await GetResult(path.join(output_folder, kern), inp, TIME_UNIT)

		let ther = await Simulation(inp)
		let ther_time = Object.keys(ther).reduce((_, v, i, a) => {
			_[mp[v]] = { start: ther[v].start * TIME_UNIT, end: (ther[v].end - 1) * TIME_UNIT }
			return _
		}, {})

		let imp = Object.keys(mp).reduce((_, v, i, a) => {
			_[mp[v]] = v
			return _
		}, {})

		// console.log('inp', inp)
		// console.log('mp', mp)
		// console.log('imp', imp)
		// console.log('exet', exet)
		// console.log('ther', ther)
		
		Judge(test, exet, ther, ther_time, imp, TIME_UNIT)
	}
}

main()
