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

var links = [];
for (var i = 0; i < data.node_list.length; i++) {
    var currentNode = data.node_list[i];
    if (!currentNode.input_edge_list) {
        continue;
    }

    for (var j = 0; j < currentNode.input_edge_list.length; j++) {
        var currentLink = currentNode.input_edge_list[j].edge;
        if (currentLink.node_index) {
            currentLink.index = j;
            currentLink.target = i;
            currentLink.source = currentLink.node_index;
            links.push(currentLink);
        }
    }
}

data.node_list[0].fixed = true;
data.node_list[0].x = 100;
data.node_list[0].y = 100;
data.node_list[1].fixed = true;
data.node_list[1].x = 100;
data.node_list[1].y = 300;


var NODE_WIDTH = 120;
var HEADER_HEIGHT = 20;

var width = 1000,
    height = 500,
    colors = d3.scale.category10();

var diagonal = d3.svg.diagonal()
    .source(function (d) {
        return {
            "x": d.source.y - d.source.boxOffset.y + HEADER_HEIGHT + 20,
            "y": d.source.x + d.source.boxOffset.x
        };
    })
    .target(function (d) {
        return {
            "x": d.target.y - d.target.boxOffset.y + HEADER_HEIGHT + 20 + 15 * d.index,
            "y": d.target.x - d.target.boxOffset.x
        };
    })
    .projection(function (d) {
        return [d.y, d.x];
    });

var force = cola.d3adaptor()
    .size([width, height])
    .linkDistance(200)
    .nodes(data.node_list)
    .links(links)
    .on('tick', function () {
        node.attr('transform', function (d) {
            return "translate(" + (d.x - d.boxOffset.x) + "," + (d.y - d.boxOffset.y) + ")";
        });
        link.attr("d", diagonal);
    });

var svg = d3.select('.graph')
    //.attr('oncontextmenu', 'return false;')
    .attr('width', width)
    .attr('height', height);

var link = svg.selectAll(".link")
        .data(force.links())
        .enter()
        .append("path")
        .attr("class", "link");

var node = svg.selectAll(".node")
    .data(force.nodes())
    .enter().append("g")
    .attr("class", "node")
    .call(force.drag);
node.append("rect")
    .attr("class", "node-header")
    .attr("width", NODE_WIDTH)
    .attr("height", HEADER_HEIGHT);
node.append("text")
    .attr("dx", ".5em")
    .attr("dy", "1em")
    .text(function (d) {
        return '[' + d.module + ']';
    });
node.append("rect")
    .attr("class", "node-body")
    .attr("y", "1em")
    .attr("width", NODE_WIDTH)
    .attr("height", function (d) {
        if (d.input_edge_list) {
            return 20 + 15 * d.input_edge_list.length;
        } else {
            return 20 + 15;
        }
    });
node.append("text")
    .attr("dx", ".5em")
    .attr("dy", HEADER_HEIGHT + 8)
    .text(function (d) {
        return d.name;
    });
node.each(function (node, i) {
    var group = d3.select(this);
    group.append("circle")
        .attr("class", "output-edge")
        .attr("r", 4)
        .attr("cx", NODE_WIDTH - 2)
        .attr("cy", HEADER_HEIGHT + 20);
    if (node.input_edge_list) {
        group.selectAll(".input-edge")
            .data(node.input_edge_list)
            .enter()
            .append("circle")
            .attr("class", "input-edge")
            .attr("r", 4)
            .attr("cx", 2)
            .attr("cy", function (d, i) {
                return HEADER_HEIGHT + 20 + 15 * i;
            });
    }
    var bbox = this.getBBox();
    node.boxOffset = {
        x: bbox.width / 2 + bbox.x,
        y: bbox.height / 2 + bbox.y
    };
});

force.start();
for (var step = 0; step < 100; step++) {
    force.tick();
}
force.stop();