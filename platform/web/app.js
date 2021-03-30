"use strict";

function createGameModule(init) {
    const module = Object.assign({}, Module);

    Object.assign(module, {
        preRun: [],
        postRun: [],

        arguments: [],

        canvas: document.getElementById('canvas'),
        status: document.getElementById('status'),
        statusDescription: document.getElementById('status-description'),

        setStatus: function(message) {
            if(module.status)
            {
                if(message.startsWith("Downloading data... ("))
                {
                    var slashPos = message.indexOf('/');

                    var numer = parseFloat(message.substring(21, slashPos - 1));
                    var denom = parseFloat(message.substring(slashPos + 1, message.length - 2));

                    module.status.innerHTML = Math.floor(100 * numer / denom) + '%';

                    var bar = document.getElementById("bar");
                    bar.style.width = Math.floor(100 * numer / denom) + "%";
                }
                else if(message == "Running...")
                {

                } else if(message == "Oops :(")
                {

                }
            }
        },

        totalDependencies: 0
    });

    Object.assign(module, init);

    return module;
}

var Module = createGameModule();