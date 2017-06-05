
// All you need to do is pass your configuration as third parameter to the chart function
document.addEventListener('DOMContentLoaded',function(){
    var now = new Date().getTime()/1000;
    var from = now - 3600*24;
    $.getJSON( "/bmedata?from="+from+"&to="+now,function(data) {
        var temperature = [];
        var pressure = [];
        var humidity = [];
        var datetime = [];
        for(var i=0;i<data.rows.length;++i) {
            if(i % 6 == 0) {
                var row = data.rows[i];
                temperature.push(row["temperature"]);
                pressure.push(row["pressure"]);
                humidity.push(row["humidity"]);
                datetime.push(row["datetime"]);
            }
        }
        var tempCtx = $("#temperature");
        var myChart = new Chart(tempCtx, {
            type: 'line',
            data: {
                labels: datetime,
                datasets: [{
                    label: 'temp',
                    data: temperature,
                    backgroundColor: "rgba(224,60,60,0.4)"
                }]
            },
            options: {
                maintainAspectRation: false,
                elements: {
                    line: {
                        tension: 0
                    }
                },
                animation: {
                    duration: 0
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            suggestedMin: 10,
                            suggestedMax: 30
                        }
                    }]
                }
            }
        });
        var pressCtx = $("#pressure");
        var myChart = new Chart(pressCtx, {
            type: 'line',
            data: {
                labels: datetime,
                datasets: [{
                    label: 'press',
                    data: pressure,
                    backgroundColor: "rgba(153,255,51,0.4)"
                }]
            },
            options: {
                maintainAspectRation: false,
                animation: {
                    duration: 0
                },
                elements: {
                    line: {
                        tension: 0
                    }
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            suggestedMin: 950,
                            suggestedMax: 1050
                        }
                    }]
                }
            }
        });
        var humCtx = $("#humidity");
        var myChart = new Chart(humCtx, {
            type: 'line',
            data: {
                labels: datetime,
                datasets: [{
                    label: 'hum',
                    data: humidity,
                    backgroundColor: "rgba(66,134,244,0.4)"
                }]
            },
            options: {
                maintainAspectRation: false,
                animation: {
                    duration: 0
                },
                elements: {
                    line: {
                        tension: 0
                    }
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            suggestedMin: 0,
                            suggestedMax: 100
                        }
                    }]
                }
            }
        });
    });
});
