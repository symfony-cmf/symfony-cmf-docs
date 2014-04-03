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

- Create a canonical link that identifies the original URL:
 ``<link rel="canonical" href="/route/org/content">``
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
            sonata_seo:
                page:
                    title: Page's default title
                    metas:
                        names:
                            description: The default description of the page
                            keywords: default, sonata, seo

    .. code-block:: php

        $container->loadFromExtension('sonata_seo', array(
            'page' => array(
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

    <html>
        <head>
            {{ sonata_seo_title() }}
            {{ sonata_seo_metadatas() }}
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

::
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
the `SeoAwareInterface` to let the bundle know, that there
some information to persist and to display on a page:

//todo work on here


Base-Usage
~~~~~~~~~~

The work of the ``SeoBundle`` id done by several interfaces. As the
``SeoAwareContent`` document implements the ``SeoAwareInterface`` to
provide some ``SeoMetadata``. That ``SeoMetada`` is the container for
the values in a seo context:

You can use that ``SeoMetadata`` by setting it to your content:

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoAwareContent;
    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata;

    // retrieve the route root node
    $routeRoot = $documentManager->find(null, '/cms/routes');

    //create the route for the document
    $route = new Route();
    $route->setPosition($routeRoot, 'seo-content');
    $route->setDefaults(
                array(
                    '_template' => 'Acme:Seo:index.html.twig'
                )
            );

    //retrieve the root document
    $rootDocument = $documentManager->find(null, '/cms/content');

    //create the seo aware document
    $seoDocument = new SeoAwareContent();
    $seoDocument->setParent($rootDocument);
    $seoDocument->setTitle('Same title as in original');
    $seoDocument->setName('seo-content');
    $seoDocument->setBody('Same Content as in Original');
    $seoDocument->addRoute($route);

    //set the seo metadata
    $seoMetadata = new SeoMetadata();
    //additional page title
    $seoMetadata->setTitle("Documents own tile");
    $seoMetadata->setMetaKeywords('Seo, Content');
    $seoMetadata->setMetaDescription(
        'This ist the text for the description meta tag'
    );
    $seoMetadata->setOriginalUrl("/original-url");
    $seoDocument->setSeoMetadata($seoMetadata);

    $manager->persist($seoDocument);
    $manager->flush();

The code adds a route with the absolute path ``/seo-content``
and a seo-content-document ``seo-content``.
This means if you visit that url you will see the
content of the document. When you have a look at the title or the
meta tags, you will see something different. The title looks like
``Documents own tile | Default title`` and the meta tags contain the
description and keywords added by some default values.
(See in the Configuration).

Strategy-Usage
~~~~~~~~~~~~~~

Instead of setting every value to the ``SeoMetadata`` manually
a strategy solution to extract the values from your content document
can be chosen. To do so you got the possibility to add strategies to
your document for each value one by one. The ``SeoPresentation`` will
loop through all available strategies, check if the document supports it
and call the `updateMetadata()` on the strategy. Depending on the
strategy a method on the document is called to extract the
value or some other work is done. It is up to you how you wanna
implement the strategy or implement the value getter on the
document.

Title-Extraction-Strategy
-------------------------

To extract your document's title to seo title the document needs to
implement the `SeoTitleInterface`. That will force your document to
implement the method `getSeoTitle()`, which will be called by the
`SeoTitleStrategy`'s `updateMetadata()` method to set the title
property on the current `SeoMetadata`.

Description-Extraction-Strategy
-------------------------------

To extract some description for the page your document is shown,
the document needs to implement the `SeoDescriptionInterface`. That
will force your content document class to implment a method called
`getSeoDescription`, which is called by the `SeoDescriptionStrategy`.

OriginalUrl-Extraction-Strategy
-------------------------------

As seen for title and description there is a way for extract the original
route from the document too. So your document just needs to implment
the `SeoOriginalRouteInterface` which forces to implement the method
`getSeoOriginalRoute`. That method should return a Cmf-Route or
a simple symfony route key to generate a absolute url. That value
will be set to a canonical Link or a redirect will be done. (depends
on your content strategy)

From SeoMetadata to MetadataTag
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

At the end of the day there will be a `SeoMetadata` container class.
A document implementing `SeoAwareInterface` will provide the pure data
as property. The strategies will fill the container in their own way.

These values will be injected into sonatas ``PageService``, which provides
some Twig-Helpers to render the data in a template:

    {{ sonata_seo_title() }}
    {{ sonata_seo_metadatas() }}
    {{ sonata_seo_link_canonical() }}
    {{ sonata_seo_lang_alternates() }}


AdminExtension - FormType
~~~~~~~~~~~~

The ``SeoBundle`` provides an admin extension and a special form
type. If you are using SonataAdmin, you should add the extension
to the ``sonata_admin.extension`` like this:

    cmf_seo.content.admin_extension:
        implements:
          - Symfony\Cmf\Bundle\SeoBundle\Model\SeoAwareInterface

This will enable the extension and you will get a new tab with
a ``SeoMetadataType`` as a form type. This should help you on
updating the documents ``SeoMetadata``.

Multlang-Support
~~~~~~~~~~~~~~~~

To get a multilang support to your document you will have to
implement the ``TranslatableInterface``. As the SeoMetadata lives
in your document that property can be translatable too. You just need to set
its mapping option to `translatable=true`.
The configuration for the default title provides a solution for multilang
support too. Instead of setting `default: Default Title`, you can set
language depending titles:

.. configuration-block::

    .. code-block:: yaml

        cmf_seo:
            title:
                default:
                    de: Default Titel
                    en: Default title

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'title' => array(
                'default'   => array(
                    'de'    => 'Default Title',
                    'en'    => 'Default title',
                ),
        ));
    .. code-block:: xml
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                    <title>
                        <default lang="de" value="Default Titel />
                        <default lang="en" value="Default title />
                    </title>
                </cmf_seo>
            </config>
        </container>


.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`with github`: git clone https://github.com/symfony-cmf/SeoContentBundle version path/to/
.. _`sonata seo documentation`: http://sonata-project.org/bundles/seo/master/doc/index.html