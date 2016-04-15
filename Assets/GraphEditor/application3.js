var data = {
    "node_list": [
        {
            // Node 0
            "module": "zooshi",
            "name": "advance_frame"
        },
        {
            // Node 1
            "module": "zooshi",
            "name": "player_entity"
        },
        {
            // Node 2
            "module": "attributes",
            "name": "get_attribute",
            "input_edge_list": [
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 0,
                        "edge_index": 0
                    }
                },
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 1,
                        "edge_index": 0
                    }
                },
                {
                    "edge_type": "Int",
                    "edge": {
                        "value": "fpl.zooshi.AttributeDef.PatronsFed"
                    }
                }
            ]
        },
        {
            // Node 3
            "module": "attributes",
            "name": "get_attribute",
            "input_edge_list": [
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 0,
                        "edge_index": 0
                    }
                },
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 1,
                        "edge_index": 0
                    }
                },
                {
                    "edge_type": "Int",
                    "edge": {
                        "value": "fpl.zooshi.AttributeDef.TargetScore"
                    }
                }
            ]
        },
        {
            // Node 4
            "module": "float_math",
            "name": "greater_than_or_equals",
            "input_edge_list": [
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 2,
                        "edge_index": 0
                    }
                },
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 3,
                        "edge_index": 0
                    }
                }
            ]
        },
        {
            // Node 5
            "module": "logic",
            "name": "if",
            "input_edge_list": [
                {
                    "edge_type": "OutputEdgeTarget",
                    "edge": {
                        "node_index": 4,
                        "edge_index": 0
                    }
                }
            ]
        }
    ]
};


// Create a new directed graph
var graph = new dagreD3.graphlib.Graph()
    .setGraph({});

// Automatically label each of the nodes
data.node_list.forEach(function(node, i) {
    graph.setNode(i + "-" + node.module + "-" + node.name, {
        label: "test"
    });
});

// Set up the edges
graph.setEdge("1-zooshi-player_entity", "2-attributes-get_attribute", { label : "link"});
/*graph.setEdge("CLOSED",     "LISTEN",     { label: "open" });
graph.setEdge("LISTEN",     "SYN RCVD",   { label: "rcv SYN" });
graph.setEdge("LISTEN",     "SYN SENT",   { label: "send" });
graph.setEdge("LISTEN",     "CLOSED",     { label: "close" });
graph.setEdge("SYN RCVD",   "FINWAIT-1",  { label: "close" });
graph.setEdge("SYN RCVD",   "ESTAB",      { label: "rcv ACK of SYN" });
graph.setEdge("SYN SENT",   "SYN RCVD",   { label: "rcv SYN" });
graph.setEdge("SYN SENT",   "ESTAB",      { label: "rcv SYN, ACK" });
graph.setEdge("SYN SENT",   "CLOSED",     { label: "close" });
graph.setEdge("ESTAB",      "FINWAIT-1",  { label: "close" });
graph.setEdge("ESTAB",      "CLOSE WAIT", { label: "rcv FIN" });
graph.setEdge("FINWAIT-1",  "FINWAIT-2",  { label: "rcv ACK of FIN" });
graph.setEdge("FINWAIT-1",  "CLOSING",    { label: "rcv FIN" });
graph.setEdge("CLOSE WAIT", "LAST-ACK",   { label: "close" });
graph.setEdge("FINWAIT-2",  "TIME WAIT",  { label: "rcv FIN" });
graph.setEdge("CLOSING",    "TIME WAIT",  { label: "rcv ACK of FIN" });
graph.setEdge("LAST-ACK",   "CLOSED",     { label: "rcv ACK of FIN" });
graph.setEdge("TIME WAIT",  "CLOSED",     { label: "timeout=2MSL" });
*/

// Set some general styles
graph.nodes().forEach(function(v) {
    var node = graph.node(v);
    node.rx = node.ry = 5;
});

// Add some custom colors based on state
//graph.node('CLOSED').style = "fill: #f77";
//graph.node('ESTAB').style = "fill: #7f7";


var width = 1000,
    height = 500;

var svg = d3.select('body')
    .append('svg')
    //.attr('oncontextmenu', 'return false;')
    .attr('width', width)
    .attr('height', height);
var inner = svg.append("graph");

// Set up zoom support
var zoom = d3.behavior.zoom().on("zoom", function() {
    inner.attr("transform", "translate(" + d3.event.translate + ")" +
        "scale(" + d3.event.scale + ")");
});
svg.call(zoom);

// Create the renderer
var render = new dagreD3.render();
render(inner, graph);

// Center the graph
var initialScale = 0.75;
zoom
    .translate([(svg.attr("width") - graph.graph().width * initialScale) / 2, 20])
    .scale(initialScale)
    .event(svg);
svg.attr('height', graph.graph().height * initialScale + 40);