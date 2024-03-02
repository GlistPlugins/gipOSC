var oscPort = new osc.WebSocketPort({
    url: "ws://localhost:8081", // URL to your Web Socket server.
    metadata: true
});


oscPort.on("message", function (oscMsg) {
	console.log("message:" + oscMsg.args[0].value);
});

oscPort.open();


function search() {
	if(document.getElementById("searchBox").value == "") return;
//	console.log("Search:" + document.getElementById("searchBox").value);

//	oscPort.on("ready", function () {
		oscPort.send({
			address: "/carrier/frequency",
			args: [
				{
					type: "s",
					value: document.getElementById("searchBox").value
				}
			]
		});
//	});
}

