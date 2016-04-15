// WEBSOCKET

var webSocket = new WebSocket("ws://localhost:9002");
webSocket.binaryType = "arraybuffer";
webSocket.onopen = function (evt) {
    window.addEventListener("beforeunload", function (event) {
        webSocket.close();
    });
};
webSocket.onmessage = function (evt) {
    processData(evt.data);
};

// RENDERING

var scene = new THREE.Scene();
var camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(100, 100, 100);
camera.lookAt(new THREE.Vector3(0, 0, 0));

var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

function render() {
	renderer.render( scene, camera );
}
render();

function processData(data) {
    for( var i = scene.children.length - 1; i >= 0; i--) {
        obj = scene.children[i];
        scene.remove(obj);
    }

    var byteArray = new Uint8Array(data);
    var buffer = new flatbuffers.ByteBuffer(byteArray);
    var DebugDrawer = Schema.Components.DebugDrawer.getRootAsDebugDrawer(buffer);

    for (var i = 0; i < DebugDrawer.capsulesLength(); i++) {
        var DebugCapsule = DebugDrawer.capsules(i);
        var origin = DebugCapsule.origin();
        var geometry = new THREE.SphereGeometry( 1 );
        var material = new THREE.MeshBasicMaterial( {color: 0xffff00} );
        var sphere = new THREE.Mesh( geometry, material );
        sphere.position.set(origin.x(), origin.y(), origin.z());
        console.log(sphere.position);
        scene.add( sphere );
    }
    render();
}
