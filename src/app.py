"""
app.py file

This file contains the main part of the Sim4SpecTerms website. It contains the Flask app and its routes.
"""
from lib2to3.fixes.fix_input import context
from lib2to3.pgen2.tokenize import group
from math import comb

from flask import Flask, render_template, request, jsonify
from flask_bootstrap import Bootstrap5
from flask_wtf import FlaskForm
from wtforms import IntegerField, FloatField, StringField
from wtforms.fields.form import FormField
from wtforms.fields.list import FieldList
from wtforms.validators import NumberRange

from src.simulator import Configurations, Groups
from src.simulator.groups import get_groups_and_configurations

app = Flask(__name__)
"""The Flask app of the Sim4SpecTerms website."""
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

    # groups, configurations = get_groups_and_configurations(s, p, d, f)

    configurations = Configurations(s, p, d, f)
    groups = Groups(configurations)

    terms = request.args.getlist("term")

    return render_template(
        'site.html',
        form=form,
        s=2, p=p, d=d, f=f,
        configurations=configurations.filter(terms=[groups.terms[int(term) - 1] for term in terms], length=PAGE_SIZE),
        terms=groups.terms,
        s_comb=comb(2, s), p_comb=comb(6, p), d_comb=comb(10, d), f_comb=comb(14, f)
    )


if __name__ == '__main__':
    app.run(threaded=True)
