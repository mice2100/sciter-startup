document.ready = function() {
    console.log("I'm ready now.");
    imgtpl.remove();
}

document.on("click", "#btn", function loadClick(evt) {
    console.log("clicked")
});
