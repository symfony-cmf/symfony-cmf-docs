.. index::
    single: Multi Language; Book

Handling Multi-Language Documents
=================================

The default storage layer of the CMF, PHPCR-ODM, can handle translations
transparently.

.. tip::

   This chapter assumes you already have an idea how to interact with
   PHPCR-ODM, which was introduced in the :doc:`Database chapter <database_layer>`.

.. caution::

    You also need the ``intl`` php extension installed and enabled (otherwise
    composer will tell you it can't find ext-intl). If you get issues that some
    locales can not be loaded, have a look at `this discussion about ICU`_.

Initial Language Choice: Lunetics LocaleBundle
----------------------------------------------

The CMF recommends to rely on the `LuneticsLocaleBundle`_
to handle requests to ``/`` on your website. This bundle provides the tools
to select the best locale for the user based on various criteria.

When you configure ``lunetics_locale``, it is recommended to use a parameter
for the locales, as you need to configure the locales for other bundles
(e.g. the CoreBundle) too.

.. configuration-block::

    .. code-block:: yaml

        lunetics_locale:
            allowed_locales: "%locales%"

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/lunetics_locale">
                <allowed-locales>%locales%</allowed-locales>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('lunetics_locale', array(
            'allowed_locales' => '%locales%',
        ));

Configuring Available Locales
-----------------------------

The CoreBundle needs to be configure with the available locales. If it is
not configured with locales, it registeres a listener that removes all
translation mapping from PHPCR-ODM documents.

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            multilang:
                locales: "%locales%"

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <multilang>%locales%</multilang>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'multilang' => array(
                'locales' => '%locales%',
            ),
        ));

PHPCR-ODM multi-language Documents
----------------------------------

You can mark any properties as being translatable and have the document
manager store and load the correct language for you. Note that translation
always happens on a document level, not on the individual translatable fields.

.. code-block:: php

    <?php

    // src/Acme/DemoBundle/Document/MyPersistentClass.php

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

    /**
     * @PHPCR\Document(translator="attribute")
     */
    class MyPersistentClass
    {
        /**
         * Translated property
         * @PHPCR\String(translated=true)
         */
        private $topic;

        // ...
    }

.. seealso::

    Read more about multi-language documents in the
    `PHPCR-ODM documentation on multi-language`_ and see
    :doc:`../bundles/phpcr_odm/multilang` for information how to configure
    PHPCR-ODM correctly.

The default documents provided by the CMF bundles are translated documents.
The CoreBundle removes the translation mapping if ``multilang`` is not
configured.

The routes are handled differently, as you can read in the next section.

Routing
-------

The ``DynamicRouter`` uses a route source to get routes that could match a
request. The concept of the default PHPCR-ODM source is to map the request URL
onto an id, which in PHPCR terms is the repository path to a node. This allows
for a very efficient lookup without needing a full search over the repository.
But a PHPCR node has exactly one path, therefore you need a separate route
document per locale. The cool thing with this is that you can localize
the URL for each language. Simply create one route document per locale.

As all routes point to the same content, the route generator can handle
picking the correct route for you when you generate the route from the
content. See also
":ref:`ContentAwareGenerator and Locales <component-route-generator-and-locales>`".

.. _book_handling-multilang_sonata-admin:

Sonata PHPCR-ODM Admin
----------------------

.. note::

    Using sonata admin is one way to make your content editable. A book
    chapter on sonata admin is planned. Meanwhile, read
    :doc:`Sonata Admin <../cookbook/creating_a_cms/sonata-admin>` in the
    "Creating a CMS" tutorial.

The first step is to configure sonata admin. You should place the
LuneticsLocaleBundle language switcher in the ``topnav`` bar.  To do this,
configure the template for the ``user_block``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            templates:
                    user_block: AcmeCoreBundle:Admin:admin_topnav.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <template user-block="AcmeCoreBundle:Admin:admin_topnav.html.twig"/>
            </config>
        </container>


    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'templates' => array(
                'user_block' => 'AcmeCoreBundle:Admin:admin_topnav.html.twig',
            ),
        ));

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
                permanent: true

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/dic/routing">

            <route id="admin_dashboard" pattern="/{_locale}/admin/">
                <default key="_controller">FrameworkBundle:Redirect:redirect</default>
                <default key="route">sonata_admin_dashboard</default>
                <default "permanent">true</default>
            </route>

            <import resource="@SonataAdminBundle/Resources/config/routing/sonata_admin.xml"
                    prefix="/{_locale}/admin"
            />

            <import resource="." type="sonata_admin" prefix="/{_locale}/admin"/>

            <!-- redirect routes for the non-locale routes -->
            <route id="admin_without_locale" pattern="/admin">
                <default key="_controller">FrameworkBundle:Redirect:redirect</default>
                <default key="route">sonata_admin_dashboard</default>
                <default "permanent">true</default>
            </route>
            <route id="admin_dashboard_without_locale" pattern="/admin/dashboard">
                <default key="_controller">FrameworkBundle:Redirect:redirect</default>
                <default key="route">sonata_admin_dashboard</default>
                <default "permanent">true</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php

        $collection = new RouteCollection();

        $collection->add('admin_dashboard', new Route('/{_locale}/admin/', array(
            '_controller' => 'FrameworkBundle:Redirect:redirect',
            'route' => 'sonata_admin_dashboard',
            'permanent' => true,
        )));

        $sonata = $loader->import('@SonataAdminBundle/Resources/config/routing/sonata_admin.xml');
        $sonata->addPrefix('/{_locale}/admin');
        $collection->addCollection($sonata);

        $sonata = $loader->import('.', 'sonata_admin');
        $sonata->addPrefix('/{_locale}/admin');
        $collection->addCollection($sonata);

        $collection->add('admin_without_locale', new Route('/admin', array(
            '_controller' => 'FrameworkBundle:Redirect:redirect',
            'route' => 'sonata_admin_dashboard',
            'permanent' => true,
        )));

        $collection->add('admin_dashboard_without_locale', new Route('/admin/dashboard', array(
            '_controller' => 'FrameworkBundle:Redirect:redirect',
            'route' => 'sonata_admin_dashboard',
            'permanent' => true,
        )));

        return $collection

Now reload the admin dashboard. The URL should be prefixed with the
default locale, for example ``/de/admin/dashboard``. When clicking on the
language switcher, the page reloads and displays the correct content for the
requested language.

If your documents implement the TranslatableInterface, you can
:ref:`configure the translatable admin extension <bundle-core-translatable-admin-extension>`
to get a language choice field to let the administrator
choose in which language to store the content.

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
