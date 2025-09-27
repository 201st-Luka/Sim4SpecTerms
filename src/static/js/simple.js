function addSpinner(elementId) {
    return function () {
        const btn = document.getElementById(elementId);
        btn.disabled = true;
        btn.innerHTML =
            '<div class="spinner-border spinner-border-sm" role="status">' +
            '  <span class="visually-hidden">Processing...</span>' +
            '</div>';
    }
}


document.getElementById("simulate-form")
    .addEventListener("submit", addSpinner("simulate-button"));
document.getElementById("filter-form")
    .addEventListener("submit", addSpinner("filter-button"));


document.addEventListener("DOMContentLoaded", function () {
    const simulateBtn = document.getElementById("simulate-button");
    const filterBtn = document.getElementById("filter-button");

    simulateBtn.disabled = false;
    filterBtn.disabled = false;
})