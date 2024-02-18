"""
app.py file

This file contains the main part of the Sim4SpecTerms website. It contains the Flask app and its routes.
"""

from math import comb

from flask import Flask, render_template, request, jsonify

from simulator import Configurations, Groups


app = Flask(__name__)
"""The Flask app of the Sim4SpecTerms website."""


@app.route('/', methods=['GET'])
def index():
    """
    Main page of website

    Returns:
        str:
            The rendered HTML template is returned as a string.
    """
    return render_template("index.html")


@app.route('/api/simulator', methods=['POST'])
def api_simulator():
    """
    API interface for the simulator

    Returns:
        flask.Response:
            The converted JSON response is returned.
    """

    # obtaining the `data` of the request (contains the parameters as a JSON/dict object)
    data = request.json.get('data')

    # parsing the options of the input form into variables
    # if this fails, the input is invalid and an exception is raised so no response is going to be returned
    s, p, d, f, range_start, range_end = (int(data['s']), int(data['p']), int(data['d']), int(data['f']),
                                          int(data['range_start']), int(data['range_end']))

    # simulating and building the terms/groups
    configurations = Configurations(s, p, d, f)
    groups = Groups(configurations)

    return jsonify(
        s=comb(2, s), p=comb(6, p), d=comb(10, d), f=comb(14, f),
        total_combs=len(configurations),
        range_start=range_start, range_end=range_end,
        rows=[conf.to_list() for conf in configurations[range_start:range_end]],
        rows_len=len(configurations),
        terms=[term.to_list() for term in groups.terms],
    )


if __name__ == '__main__':
    app.run(threaded=True)
