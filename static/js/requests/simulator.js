const rowCount = 2000;
let page = 0;


document.addEventListener('DOMContentLoaded', function () {
    const combinationsS = document.getElementById('combinations-s');
    const combinationsP = document.getElementById('combinations-p');
    const combinationsD = document.getElementById('combinations-d');
    const combinationsF = document.getElementById('combinations-f');
    const combinationsTotal = document.getElementById('combinations-total');

    const simulateButton = document.getElementById('sim-input-simulate');

    const inputForm = document.getElementById('sim-input-form');

    const simulatorIntputS = document.getElementById('sim-input-s');
    const simulatorIntputP = document.getElementById('sim-input-p');
    const simulatorIntputD = document.getElementById('sim-input-d');
    const simulatorIntputF = document.getElementById('sim-input-f');

    const rowsTable = document.getElementById('rows-table');

    simulateButton.addEventListener('click', function () {
        fetch('/api/simulator', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ data: {
                s: simulatorIntputS.value,
                p: simulatorIntputP.value,
                d: simulatorIntputD.value,
                f: simulatorIntputF.value,
                range_start: rowCount * page,
                range_end: rowCount * (page + 1)
            } }),
        })
        .then(response => response.json())
        .then(data => {
            combinationsS.innerText = `${data.s}`;
            combinationsP.innerText = `${data.p}`;
            combinationsD.innerText = `${data.d}`;
            combinationsF.innerText = `${data.f}`;
            combinationsTotal.innerText = `${data.s * data.p * data.d * data.f}`;

            while (rowsTable.rows.length > 1) {
                rowsTable.deleteRow(1);
            }

            for (let i = 0; i < data.len; ++i) {
                const tr = rowsTable.insertRow();
                const index = tr.insertCell()
                index.innerText = `${i + 1}`
                for (let j = 0; j < 18; ++j) {
                    const td = tr.insertCell();
                    td.innerText = `${data.rows[i][j]}`;
                }
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
    });

    inputForm.addEventListener('submit', function (e) {
        e.preventDefault();
    });
});
