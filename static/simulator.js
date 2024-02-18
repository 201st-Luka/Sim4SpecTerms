// paging variables
// rowCount is used to display only 2000 rows on the table
const rowCount = 2000;
// page is used to count the pages and show the rows from `(page - 1) * rowCount` to `page * rowCount`
let page = 1;


// constants of the HTML elements that are modified or used by the simulator
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
const termsTable = document.getElementById('group-table');

const pagePrevButton = document.getElementById('page-button-prev');
const pageNextButton = document.getElementById('page-button-next');
const pageLabel = document.getElementById('page-label');


// functions
/*
 * float_to_fraction is used to build a nice fraction of the floating point number of the term instead of displaying the
 * ugly floating point number
 */
function float_to_fraction(value) {
    if (Math.floor(value) === value) {
        return `${value}`;
    } else {
        return `<label class="fraction"><sub><sup>${value * 2}</sup>&frasl;<sub>2</sub></sub></label>`
    }
}

/*
 * core function of the web page
 * this function executes the request to the simulator API and displays it
 */
function simulate() {
    // modifying the buttons so they cannot be pressed again
    simulateButton.disabled = true;
    pagePrevButton.disabled = true;
    pageNextButton.disabled = true;
    simulateButton.value = "Simulating ...";

    // fetching the request
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
            range_start: rowCount * (page - 1),
            range_end: rowCount * page
        } }),
    })
    .then(response => response.json())
    .then(data => {
        // displaying the combinations
        combinationsS.innerText = `${data.s}`;
        combinationsP.innerText = `${data.p}`;
        combinationsD.innerText = `${data.d}`;
        combinationsF.innerText = `${data.f}`;
        combinationsTotal.innerText = `${data.total_combs}`;

        // enabling the page buttons if possible (if the next or previous page is not out of range)
        if (page > 1) {
            pagePrevButton.disabled = false;
        }
        if (rowCount * page < data.total_combs) {
            pageNextButton.disabled = false;
        }

        // deleting the rows that where showed previously on the tables
        while (rowsTable.rows.length > 3) {
            rowsTable.deleteRow(3);
        }
        while (termsTable.rows.length > 2) {
            termsTable.deleteRow(2);
        }

        // displaying the new configuration
        for (let i = 0; i < data.rows.length; ++i) {
            // creating a new row
            const tr = rowsTable.insertRow();
            // creating a new cell of the row
            const index = tr.insertCell()
            // displaying the index of the row
            index.innerText = `${data.range_start + i + 1}`
            for (let j = 0; j < 18; ++j) {
                // creating a new cell for the arrows and ml/ms values
                const td = tr.insertCell();
                // displaying the arrow or ml/ms value
                td.innerText = `${data.rows[i][j]}`;
            }
        }

        // displaying the new terms (same code as for the configuration)
        for (let i = 0; i < data.terms.length; ++i) {
            const tr = termsTable.insertRow();
            tr.insertCell().innerText = `${i + 1}`;

            for (let j = 0; j < 3; ++j) {
                const td = tr.insertCell();
                td.innerText = `${data.terms[i][j]}`;
            }
            const td = tr.insertCell();
            const htmlArray = [];
            for (let j = 0; j < data.terms[i][5].length; j++) {
                htmlArray[j] = `<sup>${data.terms[i][3]}</sup>${data.terms[i][4]}<sub>${float_to_fraction(data.terms[i][5][j])}</sub>`;
            }
            td.innerHTML = htmlArray.join(', ');
        }

        // enabling the button again and setting its value to indicate the simulation if over
        simulateButton.disabled = false;
        simulateButton.value = "Simulate";
    })
    .catch(error => {
        // error handling
        console.error('Error:', error);

        // enabling the button again and setting its value to indicate the simulation if over
        simulateButton.disabled = false;
        simulateButton.innerText = "Simulate";
    });
}

/*
 * render the previous page
 */
function prev_page() {
    page -= 1;
    pageLabel.innerText = `${page}`;
    simulate();
}

/*
 * render the next page
 */
function next_page() {
    page += 1;
    pageLabel.innerText = `${page}`;
    simulate();
}

/*
 * submit the input form and prevent the default event from happening
 */
function submit_input(event) {
    event.preventDefault();
    simulate();
}

/*
 * connecting the buttons and callbacks
 */
function doc_loading() {
    // resetting the page to 1
    page = 1;

    // disabling the buttons
    simulateButton.disabled = true;
    pagePrevButton.disabled = true;
    pageNextButton.disabled = true;

    // connecting the buttons and its callback functions
    pagePrevButton.addEventListener('click', prev_page);
    pageNextButton.addEventListener('click', next_page);
    inputForm.addEventListener('submit', submit_input);

    // starting the simulator once
    simulate();
}

// calling the `doc_loading` function once if the document is loaded (if the web page is opened or reloaded)
document.addEventListener('DOMContentLoaded', doc_loading);
