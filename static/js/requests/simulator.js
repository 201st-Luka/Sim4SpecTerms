const rowCount = 2000;
let page = 1;


const combinationsS = () => document.getElementById('combinations-s');
const combinationsP = () => document.getElementById('combinations-p');
const combinationsD = () => document.getElementById('combinations-d');
const combinationsF = () => document.getElementById('combinations-f');
const combinationsTotal = () => document.getElementById('combinations-total');

const simulateButton = () => document.getElementById('sim-input-simulate');

const inputForm = () => document.getElementById('sim-input-form');

const simulatorIntputS = () => document.getElementById('sim-input-s');
const simulatorIntputP = () => document.getElementById('sim-input-p');
const simulatorIntputD = () => document.getElementById('sim-input-d');
const simulatorIntputF = () => document.getElementById('sim-input-f');

const rowsTable = () => document.getElementById('rows-table');
const termsTable = () => document.getElementById('group-table');

const pagePrevButton = () => document.getElementById('page-button-prev');
const pageNextButton = () => document.getElementById('page-button-next');
const pageLabel = () => document.getElementById('page-label');


function simulate() {
    simulateButton().disable;
    pagePrevButton().disabled = true;
    pageNextButton().disabled = true;

    fetch('/api/simulator', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ data: {
            s: simulatorIntputS().value,
            p: simulatorIntputP().value,
            d: simulatorIntputD().value,
            f: simulatorIntputF().value,
            range_start: rowCount * (page - 1),
            range_end: rowCount * page
        } }),
    })
    .then(response => response.json())
    .then(data => {
        const total_combs = data.s * data.p * data.d * data.f;

        combinationsS().innerText = `${data.s}`;
        combinationsP().innerText = `${data.p}`;
        combinationsD().innerText = `${data.d}`;
        combinationsF().innerText = `${data.f}`;
        combinationsTotal().innerText = `${total_combs}`;

        if (page > 1) {
            pagePrevButton().disabled = false;
        }
        if (rowCount * page < total_combs) {
            pageNextButton().disabled = false;
        }

        while (rowsTable().rows.length > 3) {
            rowsTable().deleteRow(3);
        }
        while (termsTable().rows.length > 2) {
            termsTable().deleteRow(2);
        }

        for (let i = 0; i < data.rows.length; ++i) {
            const tr = rowsTable().insertRow();
            const index = tr.insertCell()
            index.innerText = `${data.range_start + i + 1}`
            for (let j = 0; j < 18; ++j) {
                const td = tr.insertCell();
                td.innerText = `${data.rows[i][j]}`;
            }
        }

        for (let i = 0; i < data.terms.length; ++i) {
            const tr = termsTable().insertRow();
            for (let j = 0; j < 5; ++j) {
                const td = tr.insertCell();
                td.innerText = `${data.terms[i][j]}`;
            }
        }
    })
    .catch(error => {
        console.error('Error:', error);
    });
    simulateButton().disabled = false;
}

function prev_page() {
    page -= 1;
    pageLabel().innerText = `${page}`;
    simulate();
}

function next_page() {
    page += 1;
    pageLabel().innerText = `${page}`;
    simulate();
}

function submit_input(event) {
    event.preventDefault();
    simulate();
}

function doc_loading() {
    simulateButton().disabled = true;
    pagePrevButton().disabled = true;
    pageNextButton().disabled = true;

    // simulateButton().addEventListener('click', simulate);

    pagePrevButton().addEventListener('click', prev_page);
    pageNextButton().addEventListener('click', next_page);

    inputForm().addEventListener('submit', submit_input);

    simulate();
    simulateButton().disabled = false;
}


document.addEventListener('DOMContentLoaded', doc_loading);
