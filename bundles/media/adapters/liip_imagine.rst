LiipImagine
===========

For LiipImagine, a data loader is included:
``Symfony\Cmf\Bundle\MediaBundle\Adapter\LiipImagine\CmfMediaDoctrineLoader``.
It will work for all image object implementing
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` and is automatically enabled
if the LiipImagineBundle is installed.

The dataloader has the name: ``cmf_media_doctrine_phpcr``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        liip_imagine:
            # ...
            filter_sets:
                # default filter to be used with the image preview
                image_upload_thumbnail:
                    data_loader: cmf_media_doctrine_phpcr
                    quality: 85
                    filters:
                        thumbnail: { size: [100, 100], mode: outbound }
                # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/dic/schema/liip_imagine">
                <!-- ... -->
                <!-- default filter to be used with the image preview -->
                <filter-set name="image_upload_thumbnail" data-loader="cmf_media_doctrine_phpcr" quality="85">
                    <filter name="thumbnail" size="100,100" mode="outbound"/>
                </filter-set>
                <!-- ... -->
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('liip_imagine', array(
            // ...
            'filter_sets' => array(
                // default filter to be used with the image preview
                'image_upload_thumbnail' => array(
                    'data_loader' => 'cmf_media_doctrine_phpcr',
                    'quality'     => 85,
                    'filters'     => array(
                        'thumbnail' => array(
                            'size' => array(100, 100),
                            'mode' => 'outbound',
                        ),
                    ),
                ),
                // ...
            ),
        ));
