function handleFiles(files) {
	// Check for the various File API support.
	if (window.FileReader) {
		// FileReader are supported.
		getAsText(files[0]);
	} else {
		alert('FileReader are not supported in this browser.');
	}
}
function handleSimFiles(files) {
	// Check for the various File API support.
	if (window.FileReader) {
		// FileReader are supported.
		getAsText(files[0]);
	} else {
		alert('FileReader are not supported in this browser.');
	}
}
function getAsText(fileToRead) {
	var reader = new FileReader();
	// Handle errors load
	reader.onload = loadHandler;
	reader.onerror = errorHandler;
	// Read file into memory as UTF-8      
	reader.readAsText(fileToRead);
}
function getAsText(fileToRead) {
	var reader = new FileReader();
	// Handle errors load
	reader.onload = simLoadHandler;
	reader.onerror = errorHandler;
	// Read file into memory as UTF-8      
	reader.readAsText(fileToRead);
}
function loadHandler(event) {
	var csv = event.target.result;
	processSimData(csv);             
}
function simLoadHandler(event) {
	var csv = event.target.result;
	processSimData(csv);             
}
/*
function processData(csv) {
	var allTextLines = csv.split(/\r\n|\n/);
	var lines = [];
	var nBodySimSteps = [];
	for (i=0; i<allTextLines.length-1; ++i) {
		line = allTextLines[i].split(', ');
		nBodySimSteps.push({
			time: Number(line[0]),
			particle: Number(line[1]),
			position: { x: Number(line[3]), y: Number(line[4]) },
			velocity: { x: Number(line[5]), y: Number(line[6]) },
			acceleration: { x: Number(line[7]), y: Number(line[8]) },
		});
	}

	svgDoc = document.getElementById('visual');
	grid = svgDoc.clientHeight / 10.0;
	
	var svg = d3.select("svg")
		.selectAll("svg")

	svg.data(nBodySimSteps)
		.enter()
		.append("circle")
		.attr("cx", function(d, i) { console.log(d.position.x*grid+(svgDoc.clientWidth/2)); return d.position.x*grid+(svgDoc.clientWidth/2) })
		.attr("cy", function(d, i) { console.log(d.position.y*grid+(svgDoc.clientHeight/2)); return -d.position.y*grid+(svgDoc.clientHeight/2) })
		.attr("r", 5);
}
*/
function processSimData(csv) {
	var allTextLines = csv.split(/\r\n|\n/);
	var lines = [];
	var nBodySimSteps = [];
	for (i=0; i<allTextLines.length-1; ++i) {
		line = allTextLines[i].split(', ');
		nBodySimSteps.push({
			time: Number(line[0]),
			particle: Number(line[1]),
			mass: Number(line[2]),
			position: { x: Number(line[3]), y: Number(line[4]) },
			velocity: { x: Number(line[5]), y: Number(line[6]) },
			acceleration: { x: Number(line[7]), y: Number(line[8]) },
		});
	}

	svgDoc = document.getElementById('visual');
	grid = svgDoc.clientHeight / 20.0;
	
	N = 200;
	stepArr = [];
	for (i=0; i<N; ++i) {
		stepArr.push(nBodySimSteps[0]);
		nBodySimSteps.shift();
	}
	console.log(stepArr);

	x = d3.scaleLinear()
		.domain([-1000000, 1000000])
		.range([0, svgDoc.clientWidth])
	y = d3.scaleLinear()
		.domain([-1000000, 1000000])
		.range([0, svgDoc.clientHeight])

	var svg = d3.select("#visual")
		.append("svg")
		.attr("width", svgDoc.clientWidth)
		.attr("height", svgDoc.clientHeight)
	var xAxis = svg.append("g")
		.attr('transform', 'translate(0,'+svgDoc.clientHeight/2+')')
		.call(d3.axisBottom(x))
		.style("opacity", 0.5);
	var yAxis = svg.append("g")
		.attr('transform', 'translate('+svgDoc.clientWidth/2+', 0)')
		.call(d3.axisLeft(y))
		.style("opacity", 0.5);
			
	var clip = svg.append("defs").append("SVG:clipPath")
		.attr("id", "clip")
		.append("SVG:rect")
		.attr("width", svgDoc.clientWidth )
		.attr("height", svgDoc.clientHeight )
		.attr("x", 0)
		.attr("y", 0);

  // Create the scatter variable: where both the circles and the brush take place
	var scatter = svg.append('g')
		.attr("clip-path", "url(#clip)");

	var zoom = d3.zoom()
		.scaleExtent([0, 10])
		.extent([[0, 0], [svgDoc.clientWidth, svgDoc.clientHeight]])
		.on('zoom', updateChart);
	
	svg.append('rect')
		.attr('width', svgDoc.clientWidth)
		.attr('height', svgDoc.clientHeight)
		.style('fill', 'none')
		.style('pointer-events', 'all')
		.call(zoom);

	d3.select("#zoomOut")
		.on("click", function () {
			zoom.scaleBy(scatter.transition().duration(100), 1/2);
		});
	d3.select("#zoomIn")
		.on("click", function () {
			zoom.scaleBy(scatter.transition().duration(100), 2);
		});
	d3.select("#zoomOutLittle")
		.on("click", function () {
			//zoom.scaleBy(scatter.transition().duration(100), 1/1.1);
			scatter.transition().duration(100).call(zoom.scaleBy, 0.9);
		});
	d3.select("#zoomInLittle")
		.on("click", function () {
			//zoom.scaleBy(scatter.transition().duration(100), 1.1);
			scatter.transition().duration(100).call(zoom.scaleBy, 1.3);
		});
	d3.select("#scaleA")
		.on("click", function () {
			//zoom.scaleBy(scatter.transition().duration(100), 1.1);
			scatter.transition().duration(100).call(zoom.scaleTo, 1.3);
		});
	
	for (i=0; i<N; ++i) {
	scatter
		.append("circle")
		.attr("cx", x(stepArr[i].position.x))
		.attr("cy", y(stepArr[i].position.y))
		.attr("r", stepArr[i].mass/10)
		.style("fill", "#61a3a9")
		.style("opacity", 0.5);
	}	
	
	function updateChart() {
		x = d3.event.transform.rescaleX(x);
		y = d3.event.transform.rescaleY(y);

		xAxis.call(d3.axisBottom(x));
		yAxis.call(d3.axisLeft(y));
	}

	setInterval(() => {
		console.log('iteration');
		stepArr = [];
		for (i=0; i<N; ++i) {
			stepArr.push(nBodySimSteps[0]);
			nBodySimSteps.shift();
		}
		animationStep(stepArr);
	}, 100);
	
}
function animationStep(stepArr) {
	var svg = d3.select("svg")
		.selectAll("circle")
		.transition()
		.duration(100)
		.attr("cx", function(d, i) {
			return x(stepArr[i].position.x);
		})
		.attr("cy", function(d, i) { 
			return y(stepArr[i].position.y);
		})
}

function errorHandler(evt) {
	if(evt.target.error.name == "NotReadableError") {
		alert("Canno't read file !");
	}
}

