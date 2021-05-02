import { useState, useEffect } from 'react';
import paginatorStyles from '../styles/Paginator.module.scss';

const Paginator = ({ length, pageSize, onChange }) => {
  const [pageIndex, setPageIndex] = useState(0);
  const [_pageSize, setPageSize] = useState(pageSize);
  const change = (caseEl) => {
    let maxIndex = Math.floor(length / _pageSize);
    switch (caseEl) {
      case 'left':
        setPageIndex(pageIndex - 1 < 0 ? 0 : pageIndex - 1);
        break;
      case 'left-end':
        setPageIndex(0);
        break;
      case 'right':
        setPageIndex(pageIndex + 1 > maxIndex ? maxIndex : pageIndex + 1);
        break;
      case 'right-end':
        setPageIndex(maxIndex);
        break;
      case 'input':
        if (pageIndex > maxIndex) {
          setPageIndex(maxIndex);
        } else if (pageIndex < 0) {
          setPageIndex(0);
        }
        break;
      default:
        console.log("Nobody's place");
        break;
    }
  };

  useEffect(() => {
    let pageLength = Math.min(_pageSize, length - Math.floor(pageIndex * _pageSize));
    let startIndex = pageIndex * _pageSize;
    onChange(startIndex, pageLength);
  }, [pageIndex, _pageSize]);

  return (
    <div className={paginatorStyles.container}>
      <div
        onClick={() => {
          change('left-end');
        }}
      >
        &#10096;&#10096;
      </div>
      <div
        onClick={() => {
          change('left');
        }}
      >
        &#10096;
      </div>
      <div className={paginatorStyles.pageIndex}>
        <input
          type='text'
          value={pageIndex + 1}
          onFocus={(e) => e.target.select()}
          onChange={(e) => setPageIndex(e.target.value * 1 - 1) && change('input')}
        />
        <div> &nbsp;/ {Math.ceil(length / _pageSize)}</div>
      </div>
      <div
        onClick={() => {
          change('right');
        }}
      >
        &#10097;
      </div>
      <div
        onClick={() => {
          change('right-end');
        }}
      >
        &#10097;&#10097;
      </div>
      <div>items per page: <input type="number" min="1" max="1000" value={_pageSize} onChange={(e) => setPageSize(e.target.value)}/></div>
    </div>
  );
};

Paginator.defaultProps = {
  length: 0,
  pageSize: 10
};

export default Paginator;
