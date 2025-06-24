function get(name){
    //  function that extracts the value of a query parameter from the URL
    if(name === (new RegExp('[?&]' + encodeURIComponent(name) + '=([^&]*)')).exec(location.search))
        return decodeURIComponent(name);
    else
        return 0;
}


function getHiddenFields(element) {
    return element.querySelectorAll('input[type="hidden"]');
}


function hasHiddenFields(element) {
    return getHiddenFields(element).length > 0;
}


function removeHiddenFields(element) {
    element.find('input[type="hidden"]').remove();
}


function simulate() {
    // Function submits the form and starts the simulation

    const simulator_form = $('#simulator-form')[0];

    // Get the values of the system from the URL
    const s = get('s');
    const p = get('p');
    const d = get('d');
    const f = get('f');

    if (hasHiddenFields(simulator_form)) {
        if (!($('#s')[0].value !== s || $('#p')[0].value !== p || $('#d')[0].value !== d || $('#f')[0].value !== f)) {
            // If the values in the form are different from the values in the URL, remove the hidden fields
            removeHiddenFields(simulator_form);
        }
    }

    // Submit the form
    simulator_form.submit();
}


function createHiddenFormTermElement(index) {
    return `<input type="hidden" name="term" value="${index}">`;
}


$(document).ready(function(){
    // Disable the simulate button when the form is submitted
    const simulatorForm = $('#simulator-form');
    simulatorForm.on('submit', function(){
        $('#simulate-button').prop('disabled', true);
    });

    // Add the hidden form fields to the form if the request URL query parameters contain terms
    const terms = get("term");
    console.log(terms);
});


document.addEventListener('DOMContentLoaded', function() {
    // Add event listener to the terms table
    const termsTable = $('#terms-table')[0];
    termsTable.addEventListener('click', function(event) {
        // When a row is clicked, extract the values of the row and add them to the form
        const row = event.target.parentElement;

        if (row) {
            row.classList.toggle('table-secondary');

            const simulatorForm = $('#simulator-form')[0];

            // Add/remove the term index to/from the form
            const index = row.children.item(0).textContent;
            const innerHTML = createHiddenFormTermElement(index);
            simulatorForm.innerHTML += innerHTML;

            // simulatorForm.submit();
        }
    });
});