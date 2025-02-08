from flask import Blueprint, render_template
from app.models import Item

report_bp = Blueprint('reports', __name__)

@report_bp.route('/')
def report():
    items = Item.query.all()
    return render_template('report.html', items=items)
