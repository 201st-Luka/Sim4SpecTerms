"""
app.py file

This file contains the main part of the Sim4SpecTerms website. It contains the Flask app and its routes.
"""

from math import comb
from secrets import token_urlsafe

from flask import Flask, render_template, request, jsonify
from flask_bootstrap import Bootstrap5
from flask_wtf import FlaskForm
from wtforms import IntegerField
from wtforms.validators import NumberRange

from src.simulator import Configurations, Groups

app = Flask(__name__)
"""The Flask app of the Sim4SpecTerms website."""
app.config['SECRET_KEY'] = token_urlsafe(16)
Bootstrap5(app)


PAGE_SIZE = 5000


class SimulatorForm(FlaskForm):
    s = IntegerField('s', default=0, validators=[NumberRange(min=0, max=2)])
    p = IntegerField('p', default=0, validators=[NumberRange(min=0, max=6)])
    d = IntegerField('d', default=0, validators=[NumberRange(min=0, max=10)])
    f = IntegerField('f', default=0, validators=[NumberRange(min=0, max=14)])


@app.context_processor
def utilities():
    def term_formatter(term_sup: int, term_letter: str, term_sub: list[float]):
        term_list = [
            (f"<sup>{term_sup}</sup>"
             f"{term_letter}"
             f"<sub>{int(t_sub) if t_sub.is_integer()
                 else f'<label class="fraction"><sup>{int(t_sub * 2)}</sup>&frasl;<sub>2</sub></label>'}"
             f"</sub>")
            for t_sub in term_sub
        ]

        return ", ".join(term_list)

    def arrow_formatter():
        return

    return {'term_formatter': term_formatter,
            'arrow_formatter': arrow_formatter}


@app.route('/', methods=['GET'])
def site():
    """
    Main page of website

    Returns:
        str:
            The rendered HTML template is returned as a string.
    """
    form = SimulatorForm(request.args, meta={'csrf': False})

    s, p, d, f = form.s.data, form.p.data, form.d.data, form.f.data

    configurations = Configurations(s, p, d, f)
    groups = Groups(configurations)

    return render_template(
        'site.html',
        form=form,
        configurations=enumerate(configurations[:PAGE_SIZE]), terms=enumerate(groups.terms),
        s_comb=comb(2, s), p_comb=comb(6, p), d_comb=comb(10, d), f_comb=comb(14, f)
    )


# @app.route('/api/simulator', methods=['POST'])
# def api_simulator():
#     """
#     API interface for the simulator
#
#     Returns:
#         flask.Response:
#             The converted JSON response is returned.
#     """
#
#     # obtaining the `data` of the request (contains the parameters as a JSON/dict object)
#     data = request.json.get('data')
#
#     # parsing the options of the input form into variables
#     # if this fails, the input is invalid and an exception is raised so no response is going to be returned
#     s, p, d, f, range_start, range_end = (int(data['s']), int(data['p']), int(data['d']), int(data['f']),
#                                           int(data['range_start']), int(data['range_end']))
#
#     # simulating and building the terms/groups
#     configurations = Configurations(s, p, d, f)
#     groups = Groups(configurations)
#
#     return jsonify(
#         s=comb(2, s), p=comb(6, p), d=comb(10, d), f=comb(14, f),
#         total_combs=len(configurations),
#         range_start=range_start, range_end=range_end,
#         rows=[conf.to_list() for conf in configurations[range_start:range_end]],
#         rows_len=len(configurations),
#         terms=[term.to_list() for term in groups.terms],
#     )


if __name__ == '__main__':
    app.run(threaded=True)
