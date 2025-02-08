from flask import Blueprint, render_template, redirect, url_for, flash
from app.models import db, Item
from app.forms import InventoryForm

inventory_bp = Blueprint('inventory', __name__)

@inventory_bp.route('/', methods=['GET', 'POST'])
def index():
    form = InventoryForm()
    if form.validate_on_submit():
        item_upc = form.upc.data
        item_units = form.units.data
        item_price = form.price.data
        item_url = form.url.data
        item_images = form.images.data
        item_r = form.r.data

        new_item = Item(upc=item_upc, units=item_units, price=item_price,
                        url=item_url, images=item_images, r=item_r)
        db.session.add(new_item)
        db.session.commit()
        flash('Item added successfully!', 'success')
        return redirect(url_for('inventory.index'))
    
    items = Item.query.all()
    return render_template('inventory.html', form=form, items=items)

@inventory_bp.route('/remove/<int:id>', methods=['GET', 'POST'])
def remove(id):
    item = Item.query.get_or_404(id)
    db.session.delete(item)
    db.session.commit()
    flash(f'Item with UPC {item.upc} removed from inventory!', 'danger')
    return redirect(url_for('inventory.index'))
