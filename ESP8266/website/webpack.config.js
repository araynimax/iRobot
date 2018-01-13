var webpack = require("webpack");

module.exports = {
  target: "web",
  watch: true,
  entry: './main.js',
  output: {
    path: require('path').join(__dirname, '../', 'data'),
    filename: 'app.js'
  },
  module: {
    rules: [{
        test: /\.css$/,
        use: ['style-loader', 'css-loader']
      },
      {
        test: /\.js$/,
        loader: "babel-loader",
      },
      {
        test: /\.(png|jpg|gif|svg|eot|ttf)$/,
        loader: 'file-loader',
        query: {
          name: '../website/ignore/[name].[ext]'
        },
      },
      {
        test: /\.(woff|woff2)$/,
        loader: 'file-loader',
        query: {
          name: '[name].[ext]'
        },
      },
      {
        test: /\.html$/,
        loader: 'file-loader',
        query: {
          name: '[name].[ext]'
        }
      },
    ]
  },
  plugins: [
    new webpack.ProvidePlugin({
      $: "jquery",
      jQuery: "jquery"
    })
  ]
}
