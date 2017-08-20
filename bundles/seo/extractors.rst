Using Extractors to Retrieve the Seo Metadata
=============================================

Instead of setting every value to the ``SeoMetadata`` manually, an extractor
can do the work for you. Extractors are executed when the content object
implements a specific interface. The method required by that interface will
return the value for the specific SEO data. The extractor will then update the
``SeoMetadata`` object for the current object with the returned value.

Available Extractors
--------------------

+--------------------------------+---------------------------+----------------------------------------------+
| ExtractorInterface             | Method                    | Type                                         |
+================================+===========================+==============================================+
| ``DescriptionReadInterface``   | ``getSeoDescription()``   | Returns the meta description                 |
+--------------------------------+---------------------------+----------------------------------------------+
| ``TitleReadInterface``         | ``getSeoTitle()``         | Returns the page title                       |
+--------------------------------+---------------------------+----------------------------------------------+
| -                              | ``getTitle()``            | If the document has a ``getTitle()`` method, |
|                                |                           | it'll be used as the page title              |
+--------------------------------+---------------------------+----------------------------------------------+
| ``OriginalUrlReadInterface``   | ``getSeoOriginalUrl()``   | Returns an absolute url to redirect to or    |
|                                |                           | or create a canonical link from              |
+--------------------------------+---------------------------+----------------------------------------------+
| ``OriginalRouteReadInterface`` | ``getSeoOriginalRoute()`` | Returns a ``Route`` object to redirect to    |
|                                |                           | or create a canonical link from              |
+--------------------------------+---------------------------+----------------------------------------------+
| ``ExtrasReadInterface``        | ``getSeoExtras()``        | Returns an associative array using           |
|                                |                           | ``property``, ``http-equiv`` and ``name``    |
|                                |                           | as keys (see below from an example).         |
+--------------------------------+---------------------------+----------------------------------------------+

.. note::

    The interfaces live in the ``Symfony\Cmf\Bundle\SeoBundle\Extractor``
    namespace.

An Example
----------

Assume you have an ``Article`` object and you want to use both the ``$title``
and ``$date`` properties as page title and the ``$intro`` property as
description, you can implement both interfaces and your result will be::

    // src/AppBundle/Document/Article.php
    namespace AppBundle\Document;

    use Symfony\Cmf\Bundle\SeoBundle\Extractor\TitleReadInterface;
    use Symfony\Cmf\Bundle\SeoBundle\Extractor\DescriptionReadInterface;
    use Symfony\Cmf\Bundle\SeoBundle\Extractor\ExtrasReadInterface;

    class Article implements TitleReadInterface, DescriptionReadInterface, ExtrasReadInterface
    {
        protected $title;
        protected $publishDate;
        protected $intro;

        // ...
        public function getSeoTitle()
        {
            return $this->title.' ~ '.date($this->publishDate, 'm-Y');
        }

        public function getSeoDescription()
        {
            return $this->intro;
        }

        public function getSeoExtras()
        {
            return [
                'property' => [
                    'og:title'       => $this->title,
                    'og:description' => $this->description,
                ],
            ];
        }
    }

Creating Your Own Extractor
---------------------------

To customize the extraction process, you can create your own extractor. Just
create a class which implements the ``SeoExtractorInterface`` and tag it with
``cmf_seo.extractor``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            extractor.custom:
                class: "AppBundle\Extractor\MyCustomExtractor"
                tags:
                    - { name: cmf_seo.extractor }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <service id="extractor.custom" class="AppBundle\Extractor\MyCustomExtractor">
                    <tag name="cmf_seo.extractor"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use AppBundle\Extractor\MyCustomExtractor;

        $container->register('extractor.custom', MyCustomExtractor::class)
            ->addTag('cmf_seo.extractor')
        ;
