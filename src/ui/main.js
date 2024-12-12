document.on("ready", function() {
	Window.this.state = Window.WINDOW_SHOWN;
    console.log("I'm ready now.");
    let prog = document.createElement('progress');
	document.body.appendChild(prog);
	console.log(prog.outerHTML);
	prog.max = 200;
	prog.value = 10;
	console.log(prog.outerHTML);
})

document.on("click", "#btn", function loadClick(evt) {
    console.log("clicked")
});
