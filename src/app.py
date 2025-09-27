"""
app.py file

This file contains the main part of the Sim4SpecTerms website. It contains the Flask app and its routes.
"""
import re
from math import comb

from flask import Flask, render_template, request, redirect
from flask_bootstrap import Bootstrap5
from flask_wtf import FlaskForm
from wtforms import IntegerField
from wtforms.validators import NumberRange

from simulator import analyse, simulate as sim

app = Flask(__name__)
"""The Flask app of the Sim4SpecTerms website."""
Bootstrap5(app)


PAGE_SIZE = 5000
TERM_REGEX = re.compile(r"^(?P<L>l(?P<l_value>\d+))_(?P<S>s(?P<s_value>\d+(\.\d+)?))$")


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


@app.route("/", methods=['GET'])
def index():
    """
    Index page of website

    Returns:
        str:
            The rendered HTML template is returned as a string.
    """
    return redirect("/simple")

@app.route('/simple', methods=['GET'])
def simulate():
    """
    Main page of website

    Returns:
        str:
            The rendered HTML template is returned as a string.
    """
    args = request.args

    s, p, d, f = args.get("s", 0, int), args.get("p", 0, int), args.get("d", 0, int), args.get("f", 0, int),

    configurations = sim(s, p, d, f)
    groups = analyse(configurations)

    terms = args.getlist("term")

    return render_template(
        'simple.html',
        s=s, p=p, d=d, f=f,
        configurations=configurations.filter(
            terms=[
                (int(re.match(TERM_REGEX, term).group("l_value")), float(re.match(TERM_REGEX, term).group("s_value")))
                for term in terms
            ],
            length=PAGE_SIZE
        ),
        terms=groups.terms,
        selected_terms=terms,
        s_comb=comb(2, s), p_comb=comb(6, p), d_comb=comb(10, d), f_comb=comb(14, f)
    )


if __name__ == '__main__':
    app.run(threaded=True)
