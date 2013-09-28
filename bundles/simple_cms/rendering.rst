Rendering
---------

You can specify the template to render a page by setting the
``_template`` default or by specifying the controller by setting the
``_controller`` default in the page instance. Alternatively one can
configure the template and controller also via the SimpleCmsBundle
:ref:`routing configuration <config-simple-cms-routing>`.

A simple example for such a template could look like this:

.. code-block:: jinja

    {% block content %}
        <h1>{{ page.title }}</h1>

        <div>{{ page.body|raw }}</div>

        <ul>
        {% for tag in page.tags %}
            <li>{{ tag }}</li>
        {% endfor %}
        </ul>
    {% endblock %}

If you have the CreateBundle enabled, you can also output the document with
RDFa annotations, allowing you to edit the content as well as the tags in the
frontend. The most simple form is the following twig block:

.. code-block:: jinja

    {% block content %}
        {% createphp page as="rdf" %}
            {{ rdf|raw }}
        {% endcreatephp %}
    {% endblock %}

If you want to control more detailed what should be shown with RDFa, see
chapter :doc:`../create`.