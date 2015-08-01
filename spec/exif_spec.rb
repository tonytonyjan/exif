require 'exif'

describe Exif do
  it 'works' do
    data = Exif::Data.new(File.expand_path('../sample.jpg', __FILE__))
    expect(data.model).to eq 'NIKON D600'
    expect(data.image_width).to eq 4000
    expect(data.bits_per_sample).to eq [8, 8, 8]
    # expect(data.gps_latitude).to be_within(0.0001).of(24.178028333333334)
    # expect(data.date_time).to eq Time.new(2013,12,8,21,14,11)
  end

  it 'raises errors' do
    expect{ Exif::Data.new(File.expand_path('../not_readable.jpg', __FILE__)) }.to raise_error(Exif::NotReadble)
  end
end