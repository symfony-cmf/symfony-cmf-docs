Setting Seo Metadata from Twig
==============================

.. versionadded:: 1.2
    The twig extension was added in SeoBundle 1.2.

This bundle provides a twig function ``cmf_seo_update_metadata``
which lets you populate the SEO data from an object.
For details on how populating the SEO data works, read :doc:`introduction`.

If you use this function, you must make sure to call this in your twig template
before the calls to ``sonata_seo_title`` and ``sonata_seo_metadatas`` The
recommended set up for this is to create a metadata block in your
base twig template and override it in a sub template, calling
``cmf_seo_update_metadata`` before calling ``parent()``.

.. code-block:: html+jinja

    <!-- app/Resources/views/base.html.twig -->
    <!DOCTYPE html>
    <html>
        <head>
            {% block metadata %}
                {{ sonata_seo_title() }}

                {{ sonata_seo_metadatas() }}
            {% endblock metadata %}
        </head>
        <body>
            <p>Some page body.</p>
        </body>
    </html>

    <!-- app/Resources/views/blog/post.html.twig -->
    {% extends 'base.html.twig' %}

    {% block metadata %}
        {% do cmf_seo_update_metadata(post) %}

        {{ parent() }}
    {% endblock metadata %}
