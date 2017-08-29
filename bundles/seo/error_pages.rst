Displaying Relevant Pages in Error Pages
========================================

.. versionadded:: 1.2
    The ``SuggestionProviderController`` was introduced in SeoBundle 1.2.

You don't want to lose visitors when no content is found. Instead of showing a
generic 404 error page, the SEO bundle provides the means to show potentially
relevant links to help the user find something useful.

This is implemented in the error controller. That controller uses suggestion
providers to find the most relevant pages and shows them on the error page.

Using the Exception Controller
------------------------------

When you want to add suggestions to the error pages, register the
``SuggestionProviderController`` provided by this bundle as the exception
controller:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            exception_controller: cmf_seo.error.suggestion_provider.controller:showAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/twig/twig-1.0.xsd"
        >

            <config xmlns="http://symfony.com/schema/dic/twig"
                exception-controller="cmf_seo.error.suggestion_provider.controller:showAction"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', [
            'exception_controller' => 'cmf_seo.error.suggestion_provider.controller:showAction',
        ]);

.. seealso::

    You can read more about exception controllers in the `Symfony documentation`_.

After this, you need to enable some suggestion providers. Suggestion providers
provide relevant other pages, which are displayed on the error page. The bundle
comes with two built-in providers:

``ParentSuggestionProvider``
    This provides the parent page of the not found page (e.g. ``/blog`` when
    ``/blog/foo`` was not found).
``SiblingSuggestionProvider``
    This provides the siblings of the current page (e.g. ``/blog/something``
    when ``/blog/foo`` was not found).

.. note::

    Both providers only work with PHPCR documents.

You can enable these in your config:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            error:
                enable_parent_provider:  true
                enable_sibling_provider: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/seo http://cmf.symfony.com/schema/dic/seo/seo-1.0.xsd"
        >

            <config xmlns="http://symfony.com/schema/dic/seo">
                <error enable-parent-provider="true" enable-sibling-provider="true" />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'error' => [
                'enable_parent_provider'  => true,
                'enable_sibling_provider' => true,
            ],
        ]);

.. tip::

    You can customize the template that's used for the error page. It is also
    possible to use the default exception controller for some requests. Read
    more about the available configuration settings in
    :ref:`the configuration reference <bundles-seo-config-error>`.

Creating a Custom Suggestion Provider
-------------------------------------

You can create a custom suggestion provider by implementing
``Symfony\Cmf\Bundle\SeoBundle\SuggestionProviderInterface``. This interface
requires a ``create()`` method that returns a list of routes. For instance,
assume you always want to suggest the homepage, the provider looks like::

    // src/AppBundle/Seo/HomepageSuggestionProvider.php
    namespace AppBundle\Seo;

    use Symfony\Component\Routing\Route;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Cmf\Bundle\SeoBundle\SuggestionProviderInterface;

    class HomepageSuggestionProvider implements SuggestionProviderInterface
    {
        // ...

        public function create(Request $request)
        {
            // somehow get the Route instance of the homepage route (e.g. by quering the database)
            $homepageRoute = ...;

            return [$homepageRoute];
        }
    }

Now, register this new class as a service and tag it as
``cmf_seo.suggestion_provider``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.hompage_suggestions:
                class: AppBundle\Seo\HomepageSuggestionProvider
                tags:
                    - { name: cmf_seo.suggestion_provider }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-Instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd"
        >

            <services>
                <service id="app.hompage_suggestions"
                    class="AppBundle\Seo\HomepageSuggestionProvider"
                >
                    <tag name="cmf_seo.suggestion_provider"/>
                </service>
            </services>

        </container>

    .. code-block:: php

        // app/config/services.php
        use AppBundle\Seo\HomepageSuggestionProvider;
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(HomepageSuggestionProvider::class);
        $definition->addTag('cmf_seo.suggestion_provider');
        $container->setDefinition('app.hompage_suggestions', $definition);

The tag allows a ``group`` attribute, in order to group suggested links.

.. _Symfony Documentation: https://symfony.com/doc/current/controller/error_pages.html#overriding-the-default-exceptioncontroller
