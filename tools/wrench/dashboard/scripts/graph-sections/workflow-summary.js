/*
    data: task data
*/
function getOverallWorkflowMetrics(data) {
    const tableContainer = 'overall-metrics-table'
    const taskClass = "task-details-table-td"
    document.getElementById(tableContainer).innerHTML = workflowSummaryInnerHtml
    var hosts = new Set()
    var noFailed = 0
    var noTerminated = 0
    var overallStartTime = data[0].whole_task.start
    var overallEndTime = 0
    var noTasks = data.length
    var totalReadDuration = 0
    var totalComputeDuration = 0
    var totalWriteDuration = 0
    data.forEach(function (d) {
        var currHost = d[executionHostKey]
        hosts.add(currHost)

        if (d.failed != -1) {
            noFailed++
        }

        if (d.terminated != -1) {
            noTerminated++
        }

        var whole_task = d.whole_task
        if (whole_task.start < overallStartTime) {
            overallStartTime = whole_task.start
        }
        if (whole_task.end > overallEndTime) {
            overallEndTime = whole_task.end
        }

        var readDuration = getDuration(d, "read")
        totalReadDuration += readDuration

        var computeDuration = getDuration(d, "compute")
        totalComputeDuration += computeDuration

        var writeDuration = getDuration(d, "write")
        totalWriteDuration += writeDuration
    })

    var averageReadDuration = totalReadDuration / noTasks
    var averageComputeDuration = totalComputeDuration / noTasks
    var averageWriteDuration = totalWriteDuration / noTasks

    var totalHosts = hosts.size
    var noSuccesful = noTasks - (noFailed + noTerminated)
    var totalDuration = overallEndTime - overallStartTime

    var metrics = {
        totalHosts: {
            value: totalHosts,
            display: 'Total Hosts Utilized'
        },
        totalDuration: {
            value: toFiveDecimalPlaces(totalDuration),
            display: 'Total Workflow Duration',
            unit: 's'
        },
        noTasks: {
            value: noTasks,
            display: 'Number of Tasks'
        },
        noFailed: {
            value: noFailed,
            display: 'Failed Tasks'
        },
        noTerminated: {
            value: noTerminated,
            display: 'Terminated Tasks'
        },
        noSuccesful: {
            value: noSuccesful,
            display: 'Succesful Tasks'
        },
        averageReadDuration: {
            value: toFiveDecimalPlaces(averageReadDuration),
            display: 'Average Read Duration',
            unit: 's'
        },
        averageComputeDuration: {
            value: toFiveDecimalPlaces(averageComputeDuration),
            display: 'Average Compute Duration',
            unit: 's'
        },
        averageWriteDuration: {
            value: toFiveDecimalPlaces(averageWriteDuration),
            display: 'Average Write Duration',
            unit: 's'
        }
    }

    for (var key in metrics) {
        var table = d3.select(`#${tableContainer}`)
        var tr = table.append('tr')

        var currMetric = metrics[key]
        var value = currMetric.value
        var display = currMetric.display
        var unit = currMetric.unit
        if (unit === undefined) {
            unit = ''
        }

        tr.append('td')
            .html(display)
            .attr('class', taskClass)
        tr.append('td')
            .html(`${value} ${unit}`)
            .attr('class', taskClass)
    }
}