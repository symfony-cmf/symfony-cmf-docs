Saving the SeoMetadata in the Object
====================================

The ``SeoMetadata`` can be set on your model classes, so that you can persist
it into the database. This option gives admins the possibility to change the SEO
data for the object.

In order to save the ``SeoMetadata`` in the model, the object should
implement the ``SeoAwareInterface``. This requires a getter and a setter for
the ``SeoMetadata``::

    // src/AppBundle/Document/Page.php
    namespace AppBundle\Document;

    use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;

    class Page implements SeoAwareInterface
    {
        protected $seoMetadata;

        // ...
        public function getSeoMetadata()
        {
            return $this->seoMetadata;
        }

        public function setSeoMetadata($metadata)
        {
            $this->seoMetadata = $metadata;
        }
    }

.. tip::

    Instead of implementing these methods yourself, you can also use the trait
    ``SeoAwareTrait`` to enable this behavior in your model.

Now you can set some SEO data for this ``Page`` using the metadata::

    use AppBundle\Document\Page;
    use Symfony\Cmf\Bundle\SeoBundle\SeoMetadata;

    $page = new Page();
    // ... set some page properties

    $pageMetadata = new SeoMetadata();
    $pageMetadata->setDescription('A special SEO description.');
    $pageMetadata->setTags('seo, cmf, symfony');

    $page->setSeoMetadata($pageMetadata);

Doctrine PHPCR-ODM Integration
------------------------------

In order to easily persist the SeoMetadata when using Doctrine PHPCR-ODM, the
SeoBundle provides a special ``SeoMetadata`` document with the correct
mappings. This document should be mapped as a child of the content document.

To be able to use this document, you have to enable the PHPCR persistence:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            persistence:
                phpcr: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/seo">
            <persistence>
                <phpcr />
            </persistence>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'persistence' => [
                'phpcr' => true,
            ],
        ]);

.. tip::

    This is not needed if you already enabled PHPCR on the ``cmf_core``
    bundle. See :doc:`the CoreBundle docs <../core/persistence>` for more
    information.

After you have enabled PHPCR, map ``$seoMetadata`` as a child:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Document/Page.php
        namespace AppBundle\Document;

        use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;
        use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

        /**
         * @PHPCR\Document()
         */
        class Page implements SeoAwareInterface
        {
            /**
             * @PHPCR\Child
             */
            protected $seoMetadata;

            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Page.odm.yml
        AppBundle\Document\Page:
            # ...
            child:
                # ...
                seoMetadata: ~

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Page.odm.xml -->
        <?xml ver<sion="1.0" encoding="UTF-8" ?>
        <doctrine-mapping
            xmlns="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping
            https://github.com/doctrine/phpcr-odm/raw/master/doctrine-phpcr-odm-mapping.xsd"
        >
            <document name="AppBundle\Document\Page">
                <!-- ... -->
                <child name="seoMetadata" />
            </document>
        </doctrine-mapping>

And after that, you can use the
``Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoMetadata`` document::

    // src/AppBundle/DataFixture/PHPCR/LoadPageData.php
    namespace AppBundle\DataFixtures\PHPCR;

    use AppBundle\Document\Page;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoMetadata;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $manager)
        {
            if (!$dm instanceof DocumentManager) {
                $class = get_class($dm);
                throw new \RuntimeException("Fixture requires a PHPCR ODM DocumentManager instance, instance of '$class' given.");
            }

            $page = new Page();
            // ... set some page properties

            $pageMetadata = new SeoMetadata();
            $pageMetadata->setDescription('A special SEO description.');
            $pageMetadata->setTags('seo, cmf, symfony');

            $page->setSeoMetadata($pageMetadata);

            $manager->persist($page);
            $manager->flush();
        }
    }

Doctrine ORM
------------

You can also use the Doctrine ORM with the CmfSeoBundle. Use the
``Symfony\Cmf\Bundle\SeoBundle\SeoMetadata`` class and map it as an
object:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Page.php
        namespace AppBundle\Entity;

        use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;
        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity()
         */
        class Page implements SeoAwareInterface
        {
            /**
             * @ORM\Column(type="object")
             */
            protected $seoMetadata;

            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Page.orm.yml
        AppBundle\Entity\Page:
            # ...
            fields:
                # ...
                seoMetadata:
                    type: object

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Page.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="AppBundle\Entity\Page">
                <!-- ... -->
                <field name="seoMetadata" type="object" />
            </entity>
        </doctrine-mapping>

You can also choose to put the ``SeoMetadata`` class into a separate table. To
do this, you have to enable ORM support just like you enabled PHPCR enabled
above and add a OneToOne or ManyToOne relation between the content entity and
the ``SeoMetadata`` entity.

.. _bundles-seo-metadata-form-type:

Form Type
---------

The bundle also provides a special form type called
``Symfony\Cmf\Bundle\SeoBundle\Form\Type\SeoMetadataType``. This form type can
be used in forms to edit the ``SeoMetadata`` object::

    use Sonata\AdminBundle\Form\FormMapper;
    use Symfony\Cmf\Bundle\SeoBundle\Form\Type\SeoMetadataType;

    /** @var FormMapper $formMapper */
    $formMapper
        ->add('seoMetadata', SeoMetadataType::class)
    ;

.. caution::

    The form type requires the `BurgovKeyValueFormBundle`_ to be installed and
    registered.

.. note::

    If you use Doctrine ORM, you need the form option ``by_reference`` set to
    ``false``. If you enabled the ORM backend but not the PHPCR backend, this
    option is set by default, otherwise you need to explicitly specify it in
    your ORM forms.

.. tip::

    The :doc:`SonataPhpcrAdminIntegrationBundle <../sonata_phpcr_admin_integration/seo>`
    provides an admin extension for the ``SeoAwareInterface``.

.. _`BurgovKeyValueFormBundle`: https://github.com/Burgov/KeyValueFormBundle
