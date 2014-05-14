.. index::
    single: Form Types; MediaBundle

Form Types
----------

The MediaBundle provides a couple of useful form types along with form data
transformers.

cmf_media_image
~~~~~~~~~~~~~~~

The ``cmf_media_image`` form maps to an object that implements the
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` and provides a preview of the
uploaded image.

If the `LiipImagineBundle`_ is used in your project, you can configure the
imagine filter to use for the preview, as well as additional filters to remove
from cache when the image is replaced. If the filter is not specified, it
defaults to ``image_upload_thumbnail``.

.. configuration-block::

    .. code-block:: yaml

        liip_imagine:
            # ...
            filter_sets:
                # define the filter to be used with the image preview
                image_upload_thumbnail:
                    data_loader: cmf_media_doctrine_phpcr
                    filters:
                        thumbnail: { size: [100, 100], mode: outbound }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/liip_imagine">

                <!-- define the filter to be used with the image preview -->
                <filter-set name="image_upload_thumbnail"
                    data-loader="cmf_media_doctrine_phpcr">

                    <filter name="thumbnail"
                        size="100, 100"
                        mode="outbound"
                    />
                </filter-set>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('liip_imagine', array(
            // ...

            'filter_sets' => array(
                // define the filter to be used with the image preview
                'image_upload_thumbnail' => array(
                    'data_loader' => 'cmf_media_doctrine_phpcr',
                    'filters' => array(
                        'thumbnail' => array(
                            'size' => array(100, 100),
                            'mode' => 'outbound',
                        ),
                    ),
                ),
            ),
        ));

Then you can add images to document forms as follows::

    use Symfony\Component\Form\FormBuilderInterface;

    protected function configureFormFields(FormBuilderInterface $formBuilder)
    {
         $formBuilder
            ->add('image', 'cmf_media_image', array('required' => false))
         ;
    }

.. tip::

   If you set required to ``true`` for the image, the user must re-upload a
   new image each time they edit the form. If the document must have an image,
   it makes sense to require the field when creating a new document, but make
   it optional when editing an existing document. We are
   `trying to make this automatic`_.

Next you will need to add the ``fields.html.twig`` template from the
MediaBundle to the ``form.resources``, to actually see the preview of the
uploaded image in the backend:

.. configuration-block::

    .. code-block:: yaml

        twig:
            form:
                resources:
                    - 'CmfMediaBundle:Form:fields.html.twig'

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://symfony.com/schem/dic/twig">

                <form>
                    <resource>CmfMediaBundle:Form:fields.html.twig</resource>
                </form>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'CmfMediaBundle:Form:fields.html.twig',
                ),
            ),
        ));

The document that should contain the ``Image`` document has to implement a
setter method. To profit from the automatic guesser of the form layer, the
name in the form element and this method name have to match. See
`ImagineBlock::setImage`_ for an example implementation.

To delete an image, you need to delete the document containing the image.
(There is a proposal to improve the user experience for that in a
`MediaBundle issue`_.)

.. note::

    There is a Doctrine listener to invalidate the imagine cache for the
    filters you specified. This listener will only operate when an Image is
    changed in a web request, but not when a CLI command changes images. When
    changing images with commands, you should handle cache invalidation in the
    command or manually remove the imagine cache afterwards.

.. _`LiipImagineBundle`: https://github.com/liip/LiipImagineBundle
.. _`trying to make this automatic`: https://groups.google.com/forum/?fromgroups=#!topic/symfony2/CrooBoaAlO4
.. _`ImagineBlock::setImage`: https://github.com/symfony-cmf/BlockBundle/blob/master/Doctrine/Phpcr/ImagineBlock.php#L121
.. _`MediaBundle issue`: https://github.com/symfony-cmf/MediaBundle/issues/9
