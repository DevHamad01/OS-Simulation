let steps = [];
let index = 0;
let timer = null;
let faultCount = 0;

const processColors = [
    "#38bdf8", "#f97316", "#22c55e", "#e879f9"
];

let chart;

fetch("output.json")
    .then(res => res.json())
    .then(data => {
        steps = data.steps;
        initChart();
    });

function renderStep() {
    if (index < 0 || index >= steps.length) return;

    const step = steps[index];

    document.getElementById("info").innerText =
        `Process: P${step.process} | Reference: ${step.reference} | Page Fault: ${step.page_fault}`;

    const frameDiv = document.getElementById("frames");
    frameDiv.innerHTML = "";

    step.frames.forEach(f => {
        const box = document.createElement("div");
        box.className = "frame";
        box.style.background = processColors[step.process % processColors.length];
        box.innerText = f === -1 ? "-" : f;
        frameDiv.appendChild(box);
    });

    //  EXPLANATION
    let explanation = "";

    if (step.page_fault) {
        explanation += `❌ <b>Page Fault occurred</b><br>`;
        explanation += `Page <b>${step.reference}</b> was not found in memory.<br>`;

        if (step.frames.includes(-1)) {
            explanation += `🆓 Empty frame was available, so page was loaded without replacement.<br>`;
        } else {
            explanation += `🔁 Memory was full, so a page was replaced using the selected algorithm.<br>`;
        }
    } else {
        explanation += `✅ <b>Page Hit</b><br>`;
        explanation += `Page <b>${step.reference}</b> already exists in memory. No replacement needed.<br>`;
    }

    explanation += `<br><b>Current Frame State:</b> [ ${step.frames.map(f => f === -1 ? "-" : f).join(" , ")} ]`;

    document.getElementById("explanation").innerHTML = explanation;

    //GRAPH
    if (step.page_fault) {
        faultCount++;
        chart.data.labels.push(index);
        chart.data.datasets[0].data.push(faultCount);
        chart.update();
    }
}

function nextStep() {
    if (index >= steps.length) return;
    renderStep();
    index++;
}

function prevStep() {
    index--;
    if (index < 0) index = 0;
    renderStep();
}

function autoPlay() {
    if (timer) return;
    timer = setInterval(() => {
        if (index >= steps.length) {
            pause();
            return;
        }
        nextStep();
    }, 800);
}

function pause() {
    clearInterval(timer);
    timer = null;
}

function initChart() {
    const ctx = document.getElementById("faultChart").getContext("2d");
    chart = new Chart(ctx, {
        type: "line",
        data: {
            labels: [],
            datasets: [{
                label: "Page Faults Over Time",
                data: [],
                borderColor: "#facc15",
                fill: false,
                tension: 0.3
            }]
        }
    });
}
