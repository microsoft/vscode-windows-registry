{
  "targets": [
    {
      "target_name": "winregistry",
      "conditions": [
        ['OS=="win"', {
          "sources": [
            "src/winregistry.cc"
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [
                '/Qspectre',
                '/guard:cf'
              ]
            },
            'VCLinkerTool': {
              'AdditionalOptions': [
                '/guard:cf'
              ]
            }
          },
        }]
      ]
    }
  ]
}
