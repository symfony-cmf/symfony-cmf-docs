.. index::
    single: SeoContent; Bundles
    single: SeoContentBundle

SeoContentBundle
================

    This bundle provides a solution to make content bundles
    aware for Search Engine Optimisation (SEO).

Preface
-------

As the Symfony CMF allows you to show one content on
multiple routes, it is a must have to avoid
duplicate content. There are two solutions to get rid of it:

Create a canonical link with the reference to THE
original url:

    <link rel="canonical" href="/route/org/content">

or a redirect to the original url.

Both take care on search engines, which does not like
it to have same content under different routes.

The SonataSeoBundle does a good job on handling
that stuff. You should have a look at the documentation
at http://sonata-project.org/bundles/seo/master/doc/index.html
All Solutions created by this bundle will set values to the
sonatas ``PageService`` to get them displayed with the help
of sonatas ``TwigHelper``.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository `with github`_
* Install it `with composer`_ (``symfony-Symfony CMF/seo-content-bundle`` on `Packagist`_).

As this bundle uses the SonataSeoBundle you should have a look of its `sonata seo documentation`_

Both bundles need to be registered in the ``appKernel``

    public function registerBundles()
    {
        $bundles = array(

            ...

            new \Sonata\SeoBundle\SonataSeoBundle(),
            new \Symfony\Cmf\Bundle\SeoBundle\CmfSeoBundle(),
        );

        ...

        return $bundles;
    }

Configuration
-------------

The first part of configuration is the one for the
sonata seo bundle. These settings are handled as
default values

.. configuration-block::

    .. code-block:: yaml

        sonata_seo:
            page:
                metas:
                    names:
                        description: default description
                        keywords: default, key, other

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



Without any settings or work with the SeoBundle these settings
are enough to let the sonatas ``PageService`` know about your
defaults. These configs would cause a page title, a metatag for
description and one for the keywords, if you add the following
helpers to your html-head in your template:

    {{ sonata_seo_title() }}
    {{ sonata_seo_metadatas() }}

The SeoBundle adds some more options:

.. configuration-block::

    .. code-block:: yaml

        cmf_seo:
            title:
                strategy: append
                separator: ' | '
            content:
              strategy: canonical

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'title' => array(
                'default'   => 'default',
                'strategy'  => 'append',
                'separator' => ' | ',
                ),
            'content' => array(
                'strategy'  => 'canonical',
                ),
        ));
    .. code-block:: xml
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                    <title
                        default="Default title"
                        strategy="append"
                        separator=" | "
                    >
                    <content
                        strategy="canonical"
                    />
                </cmf_seo>
            </config>
        </container>

Now you are able to append or prepend a title to the default value.
Even replacing it by your title is possible. That means you should
set the ``cmf_seo.title.strategy`` either to `prepend` (default),
``append`` or ``replace``. Your values for description and keywords
will be appended to the sonatas default ones by a ". " or a ", ".
The ``cmf_seo.title.separator`` will configures the string separator
for appending or prepending the title.
In case of duplicate content you will need the ``cmf_seo.content.strategy``
The allowed values are ``canonical`` or ``redirect``. First one will
cause a canonical link, the last forces a redirect to the original
url.

Base-Usage
~~~~~~~~~~

The work of the ``SeoBundle`` id done by several interfaces. As the
``SeoAwareContent`` document implements the ``SeoAwareInterface`` to
provide some ``SeoMetadata``. That ``SeoMetada`` is the container for
the values in a seo context:

     /**
     * This string contains the information where we will find the original content.
     * Depending on the setting for the cmf_seo.content.strategy, we will do an redirect to this url or
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

Same as for title.

OriginalUrl-Extraction-Strategy
-------------------------------

Same as for title.

From SeoMetadata to MetadataTag
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

At the end of the day there will be a `SeoMetadata` container class.
A document implementing `SeoAwareInterface` will provide the pure data
as property. The strategies will ... todo go further ...
Sonatas ``PageService`` works like a container
and is able to store these values for us. When adding one of sonatas
twig helpers

    {{ sonata_seo_title() }}
    {{ sonata_seo_metadatas() }}
    {{ sonata_seo_link_canonical() }}
    {{ sonata_seo_lang_alternates() }}

sonatas SeoBundle is able to create metatags from some values or setting
the content to the title tag. Without adding any ``SeoMetadata`` to
the document, sonatas SeoBundle would render the default values
served in their own config block. Symfony CMF's SeoBundle has a
Listener, which takes care on the documents. If there is a
content of type ``SeoAwareInterface`` a ``SeoPresentation``-Model
starts to work and fills sonatas PageService depending on the
strategies set in the SeoBundle's configuration block.


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

.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`with github`: git clone https://github.com/symfony-cmf/SeoContentBundle version path/to/
.. _`sonata seo documentation`: http://sonata-project.org/bundles/seo/master/doc/index.html