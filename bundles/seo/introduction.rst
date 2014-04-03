SeoContentBundle
================

    This bundle helps you optimize your websites
    for search engines by collecting SEO data in
    a central location and making it available in
    twig. It is built on top of the SonataSeoBundle.

Preface
-------

Search engines punish you when you provide
the same content under several URLs.
The CMF allows you to have several URLs
for the same content if you need that.
There are two solutions to avoid penalties
with search engines:

- Create a canonical link that identifies the original URL: ``<link rel="canonical" href="/route/org/content">``

- Redirect to THE original url.

Both take care on search engines, which does not like
it to have same content under different routes.

The SeoBundle uses sonatas SeoBundle and its TwigHelper
to render the the `SeoMetadata` into your Pag. So you
should have a look at the documentation
at `sonata seo documentation_`

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository `with github`_
* Install it `with composer`_ (``symfony-Symfony CMF/seo-content-bundle`` on `Packagist`_).

Both bundles need to be registered in the ``appKernel``

.. code-block:: php

    // app/appKernel.php
    public function registerBundles()
    {
        $bundles = array(
            //register both SeoBundles
            new Sonata\SeoBundle\SonataSeoBundle(),
            new Symfony\Cmf\Bundle\SeoBundle\CmfSeoBundle(),
        );

        return $bundles;
    }

A very basic use case
_____________________

The simplest use case would be to just set some configuration
to the sonata_seo configuration section and set the TwigHelper
into your template.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_seo:
            page:
                title: Page's default title
                metas:
                    names:
                        description: The default description of the page
                        keywords: default, sonata, seo

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_seo', array(
            'page' => array(
                'title' => 'Page's default title',
                'metas' => array(
                    'names' => array(
                        'description' => 'default description',
                        'keywords' => 'default, key, other',
                    ),
                ),
            ),
        ));

To get a deeper look into the sonata seo-bundle configuration you
should visit its documentation at `sonata seo documentation_`
The only thing to do now is to insert the TwigHelper into your
template:

.. code-block:: html

    <!-- app/Resources/views/base.html.twig -->
    <html>
        <head>
            {{ sonata_seo_title() }}
            {{ sonata_seo_metadatas() }}
            {{ sonata_seo_link_canonical() }} // needed later
        </head>
        <body>
            <p>Some page body.</p>
        </body>
    </html>

This will render a Page with the tilte defined above. The
information definded for description and keywords will go
into the meta tags.

Using SeoMetadata - Admin extension and a form type
---------------------------------------------------

The basic example would work perfect without the Symfony CMF
SeoBundle. But the SeoBundle creates more possibilities to
create the pages's title, description, keywords and even
the original url for a canonical link or a redirect.
To persist that data the Bundle serves a value object
called `SeoMetada`:

.. code-block:: php

     /**
     * This string contains the information where we will find the original content.
     * Depending on the setting for the cmf_seo.content.pattern, we will do an redirect to this url or
     * create a canonical link with this value as the href attribute.
     *
     * @var string
     */
    private $originalUrl;

    /**
     * If this string is set, it will be inserted as a meta tag for the page description.
     *
     * @var  string
     */
    private $metaDescription;

    /**
     * This comma separated list will contain the Keywords for the page's meta information.
     *
     * @var string
     */
    private $metaKeywords;

A object should implement
the `SeoAwareInterface`, which simply forced to implement setter/getter for the
seo metadata. A simple example would be:

.. code-block:: php

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoAwareContent;
    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata;

    //retrieve the root document
    $rootDocument = $documentManager->find(null, '/cms/content');

    //create the seo aware document
    $seoDocument = new SeoAwareContent();
    $seoDocument->setParent($rootDocument);
    $seoDocument->setTitle('Same title as in original');
    $seoDocument->setName('seo-content');
    $seoDocument->setBody('Same Content as in Original');

    //set the seo metadata
    $seoMetadata = new SeoMetadata();
    $seoMetadata->setTitle("Documents own tile");
    $seoMetadata->setMetaKeywords('Seo, Content');
    $seoMetadata->setMetaDescription(
        'This ist the text for the description meta tag'
    );
    $seoMetadata->setOriginalUrl("/original/url/of/content");
    $seoDocument->setSeoMetadata($seoMetadata);

    // retrieve the route root node
    $routeRoot = $documentManager->find(null, '/cms/routes');

    //create the route for the document
    $route = new Route();
    $route->setParentDocument($routeRoot);
    $route->setName('seo-content');
    $route->setContent($soDocument);
    $route->setDefault('_template', '::base.html.twig');

    $manager->persist($seoDocument);
    $manager->persist($route);
    $manager->flush();

Visiting the site with the url ``/seo-content`` (same template shown above) will
show a Page with "Documents own tile" as title, "This ist the text for the description
meta tag" in the description, "Seo, Content" in the keywords and a canonical link with
``href="/original/url/of/content"``. But what about some default string to just concatenate
defaults and documents own values? Just add some more configs to the cmf_seo configuration
section.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_seo:
            page:
                metas:
                    names:
                        keywords: default, sonata, seo
        cmf_seo:
            title: default_title_key
            description: default_title_key

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    title="default_title_key"
                    description="default_title_key">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'sonata_seo', array(
                'page' => array(
                    'metas' => array(
                        'names' => array(
                            'keywords' => 'default, key, other',
                        ),
                    ),
                ),
            ),
            'cmf_seo' => array(
                'title'         => 'default_title_key',
                'description'   => 'default_description_key',
            ),
        );

As you will notice, you got the opportunity to set Symfony translation key for your
default values for title and description. So you will got Multi-Language-Support
out of the box. Just define your values for default title/description as translations:

.. code-block:: xml

    <!-- app/Resources/translations/messages.en.xliff -->
    <?xml version="1.0" encoding="utf-8"?>
    <xliff xmlns="urn:oasis:names:tc:xliff:document:1.2" version="1.2">
        <file source-language="en" target-language="en" datatype="plaintext" original="messages.en.xliff">
            <body>
                <trans-unit id="default_title_key">
                    <source>default_title_key</source>
                    <target>%content_title% | Default title</target>
                </trans-unit>
                <trans-unit id="default_description_key">
                    <source>default_description_key</source>
                    <target>Default description. %content_description%</target>
                </trans-unit>
            </body>
        </file>
    </xliff>

If you want to concatenate your documents values with the default ones you need them as
parameters in you translation target.

.. tip::

    If you does not what to open a translation file for two entry, just set
    ``Default title | %%content_title%%``or ``Default description. %%content_description%%``.

For changing the default translation domain (messages), the SeoBundle provides a configuration
value:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            translation_domain: AcmeDemoBundle

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    translation-domain="AcmeDemoBundle">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => array(
                'translation_domain'         => 'AcmeDemoBundle',
            ),
        );

For redirects instead of canonical links (default) set the following option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            original_route_pattern: redirect

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    original-route-pattern="redirect">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => array(
                'original_route_pattern'    => 'redirect',
            ),
        );

This value will cause a redirect to the url persisted in the ``originalUrl`` property of the
``SeoMetadata``.

The SeoMetadata contains a form type for your Symfony Form. Just create you form with the following key:

.. code-block:: php

    $formBuilder
        ...
        ->add('seoMetadata', 'seo_metadata', array('label' => false));
        ...
        ;

For SonataAdminBundle user the SeoBundle provides an admin extension to add that form to your
form configuration.

Using extractors for getting your documents seo metadata
--------------------------------------------------------

Instead of setting every value to the ``SeoMetadata`` manually
a strategy solution to extract the values from your content document
can be chosen. To do so you got the possibility to add strategies to
your document for each value one by one. Depending on the
strategy a method on the document is called to extract the
value. It is up to the developer how to implement that extraction methods.

+--------------------------+------------------------+-----------------------------------------------+
|StrategyInterface         |  method call           | Description                                   |
+==========================+========================+===============================================+
|SeoDescriptionExtractor   |  getSeoDescription()   | the documents part for the page description   |
+--------------------------+------------------------+-----------------------------------------------+
|SeoOriginalRouteExtractor | getSeoOriginalRoute()  |return a ``Route`` object to redirect to       |
|                          |                        |or create a canonical link from                |
+--------------------------+------------------------+-----------------------------------------------+
|SeoOriginalUrlExtractor   | getSeoOriginalUrl()    |return a absolute url object to redirect to    |
|                          |                        |or create a canonical link from                |
+--------------------------+------------------------+-----------------------------------------------+
|SeoTitleExtractor         | getSeoTitle()          |return a string for setting the page title     |
+--------------------------+------------------------+-----------------------------------------------+
|TitleReadExtractor        | -                      |if implemented the ``getTitle()`` the          |
|                          |                        |extractor will use this                        |
+--------------------------+------------------------+-----------------------------------------------+

For customizing the extraction process you have got the opportunity to create your own extractor.
Just by implementing the ``SeoExtractorInterface`` and tagging the service as ``cmf_seo.extractor``

.. code-block:: xml

    <?xml version="1.0" ?>

    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <parameters>
            <parameter key="acme_demo.extractor_strategy.title.class">Acme\DemoBundle\Extractor\MyTitleExtractor</parameter>
        </parameters>

        <services>
            <service id="acme_demo.extractor_strategy.title" class="%acme_demo.extractor_strategy.title.class%">
                <tag name="cmf_seo.extractor"/>
            </service>
        </services>

    </container>

.. _`with composer`: http://getcomposer.org
.. _`packagist`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`with github`: git clone https://github.com/symfony-cmf/SeoContentBundle version path/to/
.. _`sonata seo documentation`: http://sonata-project.org/bundles/seo/master/doc/index.html