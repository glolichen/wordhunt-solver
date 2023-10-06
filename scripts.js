let wordList = "";
let xhr = new XMLHttpRequest();
xhr.open("GET", "./wordlist.txt", false);
xhr.send();
if (xhr.status == 200) {
	wordList = xhr.responseText.split("\n").join(" ");
	Module["onRuntimeInitialized"] = () => {
		Module.init(wordList);
	};
}

var xMax = 0, yMax = 0;

var table = document.getElementById("inputTable");
function fillTable(sizeX, sizeY) {
	table.innerHTML = "";
	xMax = sizeX, yMax = sizeY;
	for (let i = 0; i < sizeX; i++) {
		const tr = document.createElement("tr");
		for (let j = 0; j < sizeY; j++) {
			const td = document.createElement("td");
			const input = document.createElement("input");
			input.type = "text";
			input.maxLength = 1;
			input.id = `input${i * sizeY + j}`;
			input.onkeydown = e => {
				if (e.key != "Backspace") {
					setTimeout(() => {
						let id = input.id;
						id = parseInt(id.split("input")[1]);
						id = "input" + (id + 1);
						document.getElementById(id)?.focus();
					}, 50);
				} else {
					setTimeout(() => {
						let id = input.id;
						id = parseInt(id.split("input")[1]);
						id = "input" + (id - 1);
						document.getElementById(id)?.focus();
					}, 50);
				}
			};
			td.appendChild(input);
			tr.appendChild(td);
		}
		table.appendChild(tr);
	}
}

const ARROWS = [ "↑", "↗", "→", "↘", "↓", "↙", "←", "↖" ];

var out = document.getElementById("output");
function submit() {
	out.textContent = "";
	let grid = "";
	for (let i = 0; i < xMax * yMax; i++)
		grid += document.getElementById(`input${i}`).value;
	var ans = Module.search(grid.toUpperCase(), xMax, yMax);
	for (let i = ans.size() - 1; i >= 0; i--) {
		out.textContent += `${ans.get(i).word} [${ans.get(i).startY + 1}, ${yMax - ans.get(i).startX}]`;
		for (let j = 0; j < ans.get(i).movement.size(); j++) {
			out.textContent += " " + ARROWS[ans.get(i).movement.get(j)];
		}
		out.textContent += "\n";
	}
}

// https://stackoverflow.com/a/24457420
function isNumeric(value) {
    return /^-?\d+$/.test(value);
}

var clearButton = document.getElementById("clear");
function clear() {
	for (let i = 0; i < xMax * yMax; i++)
		document.getElementById(`input${i}`).value = "";
}
clearButton.onclick = () => clear();

var resizeButton = document.getElementById("resize");
function resize() {
	var x = "", y = "";
	while (!isNumeric(x))
		x = window.prompt("x size");
	while (!isNumeric(y))
		y = window.prompt("y size");
	fillTable(parseInt(x), parseInt(y));
}
resizeButton.onclick = () => resize();

fillTable(4, 4);