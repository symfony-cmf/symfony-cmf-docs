Handling Multilang Documents
============================
The goal of the tutorial / short introduction...
Something you need to read first: :ref:`phpcr-odm-multilang-config`


Make fields translatable
------------------------
Coming soon.


Adding language switcher
------------------------
The first step is to configure sonata admin. We are going to place the language switcher in the topnav bar.
To do this we need to configure the template for the "user_block" as shown below:

.. code-block:: yaml

    # app/main/config/config.yml
    sonata_admin:
        ...
        templates:
                user_block: LiipchCoreBundle:Admin:admin_topnav.html.twig


.. code-block:: jinja

    {#src/Liipch/CoreBundle/Resources/views/Admin/admin_topnav.html.twig#}
    {% extends 'SonataAdminBundle:Core:user_block.html.twig' %}

    {% block user_block %}
        {{ locale_switcher(null, null, 'LiipchCoreBundle:Admin:switcher_links.html.twig') }}
        {{ parent() }}
    {% endblock %}


.. code-block:: jinja

    {#src/Liipch/CoreBundle/Resources/views/Admin/switcher_links.html.twig#}
    Switch to :
    {% for locale in locales %}
        {% if loop.index > 1 %} | {% endif %}<a href="{{ locale.link }}" title="{{ locale.locale_target_language }}">{{ locale.locale_target_language }}</a>
    {% endfor %}


The next step is to update the routing.yml with the following content:

.. code-block:: yaml

    # app/main/config/routing.yml
    admin_wo_locale:
        pattern: /admin
        defaults:
            _controller: FrameworkBundle:Redirect:redirect
            route: sonata_admin_dashboard
            permanent: true # this for 301

    admin_dashboard_wo_locale:
        pattern: /admin/dashboard
        defaults:
            _controller: FrameworkBundle:Redirect:redirect
            route: sonata_admin_dashboard
            permanent: true # this for 301

    admin_dashboard:
        pattern: /{_locale}/admin/
        defaults:
            _controller: FrameworkBundle:Redirect:redirect
            route: sonata_admin_dashboard
            permanent: true # this for 301

    admin:
        resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
        prefix: /{_locale}/admin

    sonata_admin:
        resource: .
        type: sonata_admin
        prefix: /{_locale}/admin

That should already do the trick.

Reference
---------
