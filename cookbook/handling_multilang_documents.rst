.. index::
    single: Multi Language Documents

Handling Multi-Language Documents
=================================

.. include:: _outdate-caution.rst.inc

The goal of the tutorial is to describe all the steps that are needed to be
taken to use multi-language documents as clearly as possible.

Please read this first
----------------------

You need to understand how to use PHPCR-ODM. You find an introduction in
:doc:`Documentation on the PHPCR-ODM Doctrine bundle <../bundles/phpcr_odm>`

Lunetics LocaleBundle
---------------------

The CMF recommends to rely on the `LuneticsLocaleBundle`_
to handle initial locale selection when a user first visits the site,
and to provide a locale switcher.

To install the bundle, require it in your project with:

.. code-block:: bash

    $ php composer.phar require lunetics/locale-bundle

and then instantiate ``Lunetics\LocaleBundle\LuneticsLocaleBundle`` in your
``AppKernel.php``.

You also need the ``intl`` php extension installed and enabled. (Otherwise
composer will tell you it can't find ext-intl.) If you get issues that some
locales can not be loaded, have a look at `this discussion about ICU`_.

Then configure it in the main application configuration file. As there are
several CMF bundles wanting the list of allowed locales, we recommend putting
them into a parameter ``%locales%``, see the `cmf-sandbox config.yml file`_
for an example.

.. tip::

    Whenever you do a sub-request, for example to call a controller from a twig
    template, do not forget to pass the ``app.request.locale`` along or you will
    lose the request locale and fall back to the default.
    See for example the action to include the create.js JavaScript files in the
    :ref:`create.js reference <bundle-create-usage-embed>`.

PHPCR-ODM multi-language Documents
----------------------------------

You can mark any properties as being translatable and have the document
manager store and load the correct language for you. Note that translation
always happens on a document level, not on the individual translatable fields.

.. code-block:: php

    <?php

    /**
     * @PHPCRODM\Document(translator="attribute")
     */
    class MyPersistentClass
    {
        /**
         * Translated property
         * @String(translated=true)
         */
        private $topic;

        // ...
    }

Read more about multi-language documents in the
`PHPCR-ODM documentation on multi-language`_ and see
:ref:`bundle-phpcr-odm-multilang-config` to configure PHPCR-ODM correctly.

Most of the CMF bundles provide multi-language documents, for example
``MultilangStaticContent``, ``MultilangMenuNode`` or ``MultilangSimpleBlock``.
The routing is different, as explained in the next section.

Routing
-------

The ``DynamicRouter`` uses a route source to get routes that could match a
request. The concept of the default PHPCR-ODM source is to map the request URL
onto an id, which in PHPCR terms is the repository path to a node. This allows
for a very efficient lookup without needing a full search over the repository.
But a PHPCR node has exactly one path, therefore we need a separate route
document for each locale. The cool thing with this is that we can localize
the URL for each language. Simply create one route document per locale,
and set a default value for ``_locale`` to point to the locale of that route.

As all routes point to the same content, the route generator can handle
picking the correct route for you when you generate the route from the
content. See also
":ref:`ContentAwareGeneator and Locales <component-route-generator-and-locales>`".

Sonata PHPCR-ODM Admin
----------------------

This section explains how to make Sonata Admin handle multi-language
documents. You should already have set up Sonata PHPCR-ODM Admin and
understand how it works, see
:doc:`Creating a CMS using the CMF and Sonata <creating_cms_using_cmf_and_sonata>`.

.. note::

    The following assumes that you installed the LuneticsLocaleBundle as
    explained above. If you want to use something else or write your own
    locale handling, first think if it would not make sense to give the
    Lunetics bundle a try. If you are still convinced you will need to adapt
    the following template examples to your way of building a locale switcher.

The first step is to configure sonata admin. We are going to place the
LuneticsLocaleBundle language switcher in the ``topnav`` bar.  To do this we
need to configure the template for the ``user_block`` as shown below:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            templates:
                    user_block: AcmeCoreBundle:Admin:admin_topnav.html.twig

And the template looks like this:

.. code-block:: jinja

    {# src/Acme/CoreBundle/Resources/views/Admin/admin_topnav.html.twig #}
    {% extends 'SonataAdminBundle:Core:user_block.html.twig' %}

    {% block user_block %}
        {{ locale_switcher(null, null, 'AcmeCoreBundle:Admin:switcher_links.html.twig') }}
        {{ parent() }}
    {% endblock %}

You need to tell the ``locale_switcher`` to use a custom template to display
the links, which looks like this:

.. code-block:: jinja

    {# src/Acme/CoreBundle/Resources/views/Admin/switcher_links.html.twig #}
    Switch to :
    {% for locale in locales %}
        {% if loop.index > 1 %} | {% endif %}<a href="{{ locale.link }}" title="{{ locale.locale_target_language }}">{{ locale.locale_target_language }}</a>
    {% endfor %}

Now what is left to do is to update the sonata routes to become locale aware:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

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

        # redirect routes for the non-locale routes
        admin_without_locale:
            pattern: /admin
            defaults:
                _controller: FrameworkBundle:Redirect:redirect
                route: sonata_admin_dashboard
                permanent: true # this for 301

        admin_dashboard_without_locale:
            pattern: /admin/dashboard
            defaults:
                _controller: FrameworkBundle:Redirect:redirect
                route: sonata_admin_dashboard
                permanent: true # this for 301

When we now reload the admin dashboard, the url should be prefixed with our
default locale, for example ``/de/admin/dashboard``. When clicking on the
language switcher the page reloads and displays the correct content for the
requested language.

The provided sonata admin classes map the locale field of the multi-language
documents to the form. You need to do the same in your admins, in order to
create new translations. Otherwise the language fallback of PHPCR-ODM will
make you update the original language, even when you request a different
locale.  With the mapped locale field, the editor can choose if they need to
create a new language version or updates the loaded one.

Frontend Editing and multi-language
-----------------------------------

When using the CreateBundle, you do not need to do anything at all to get
multi-language support. PHPCR-ODM will deliver the document in the requested
language, and the callback URL is generated in the request locale, leading to
save the edited document in the same language as it was loaded.

.. note::

    If a translation is missing, language fallback kicks in, both when viewing
    the page but also when saving the changes, so you always update the
    current locale.

    It would make sense to offer the user the choice whether they want to
    create a new translation or update the existing one. There is this
    `issue`_ in the CreateBundle issue tracker.

.. _`LuneticsLocaleBundle`: https://github.com/lunetics/LocaleBundle/
.. _`this discussion about ICU`: https://github.com/symfony/symfony/issues/5279#issuecomment-11710480
.. _`cmf-sandbox config.yml file`: https://github.com/symfony-cmf/cmf-sandbox/blob/master/app/config/config.yml
.. _`PHPCR-ODM documentation on multi-language`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html
.. _`issue`: https://github.com/symfony-cmf/CreateBundle/issues/39
